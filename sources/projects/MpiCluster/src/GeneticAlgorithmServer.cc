#include <boost/mpi.hpp>
#include "GeneticAlgorithmServer.hh"
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
  std::cout<<"GeneticAlgorithmServer started"<<std::endl;
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
      std::cout<<"genetic received "<<inMessage.getNodeId()<<" -- size="<<mQueue->size()<<std::endl;
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

  std::cout<<"GeneticServer processQueue"<<std::endl;
  while(true)
  {
    m.lock();
    // std::cout<<"proc "<<mQueue->size()<<std::endl;
    if(mQueue->size() > 0)
    {
      inMessage = mQueue->front();
      mQueue->pop();
      int id = inMessage.getNodeId();
      GeneticAlgorithmReply outMessage(id);
      world.send(0, 0, outMessage);
      std::cout<<"genetic send "<<outMessage.getNodeId()<<std::endl;
    }
    m.unlock();
  }
}

