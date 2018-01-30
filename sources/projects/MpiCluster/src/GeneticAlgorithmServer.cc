#include <boost/mpi.hpp>
#include "GeneticAlgorithmServer.hh"
#include "GeneticAlgorithmController.hh"
#include "GeneticAlgorithmParameterContainer.hh"
#include <iostream>
#include <mutex>

std::mutex m;

GeneticAlgorithmServer::GeneticAlgorithmServer()
{
  mQueue = new std::queue<GeneticAlgorithmMessage>;
  receiverThread = NULL;
}

GeneticAlgorithmServer::~GeneticAlgorithmServer()
{
  delete mQueue;
  if(receiverThread != NULL)
  {
    delete receiverThread;
  }
}

void GeneticAlgorithmServer::start()
{
  // std::cout<<"GeneticAlgorithmServer started"<<std::endl;
  receiverThread = new std::thread(&GeneticAlgorithmServer::receiver, this);
  processQueue();
}

void GeneticAlgorithmServer::receiver()
{
  int argc;
  char **argv = NULL;
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;
  GeneticAlgorithmMessage inMessage;
  boost::mpi::status status;

  while(true)
  {
    status = world.recv(boost::mpi::any_source, boost::mpi::any_tag, inMessage);
    if(status.tag() == 0)
    {
      m.lock();
      mQueue->push(inMessage);
      // std::cout<<"  -genetic received("<<world.rank()<<") "<<inMessage.getNodeId()<<" -- size="<<mQueue->size()<<std::endl;
      m.unlock();
    }
    else if(status.tag() == 1)
    {
      GeneticAlgorithmReply outMessage;
      world.send(0, 1, outMessage);
    }
  }
}

void GeneticAlgorithmServer::processQueue()
{
  int argc;
  char **argv = NULL;
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;
  GeneticAlgorithmMessage inMessage;

  // std::cout<<"GeneticServer processQueue"<<std::endl;
  while(true)
  {
    m.lock();
    if(mQueue->size() > 0)
    {
      inMessage = mQueue->front();
      mQueue->pop();

      SerializedNetwork serializedNetwork = inMessage.mNetwork;
      Network* network = new Network;
      deserializeNetwork(&serializedNetwork, network);

      SerializedVectorField serializedVectorField = inMessage.mVectorField;
      VectorField* targetVectorField = new VectorField;
      deserializeVectorField(&serializedVectorField, targetVectorField);

      GeneticAlgorithmParameterContainer *geneticParameters = new GeneticAlgorithmParameterContainer;
      geneticParameters->modificationTypeProbabilities = hebbianModTypeProbabilities;
      GeneticAlgorithmController geneticController(geneticParameters);
      geneticController.fitToVectorField(network, targetVectorField);
      delete geneticParameters;

      SerializedNetwork serializedModifiedNetwork;
      serializeNetwork(network, &serializedModifiedNetwork);

      int id = inMessage.getNodeId();
      GeneticAlgorithmReply outMessage(id);
      outMessage.mNetwork = serializedModifiedNetwork;
      world.send(0, 0, outMessage);
      // std::cout<<"genetic send "<<outMessage.getNodeId()<<std::endl;
      
      delete network;
      delete targetVectorField;
    }
    m.unlock();
  }
}

