#include <boost/mpi.hpp>
#include <thread>
#include <mutex>
#include "MultilayerNetworkServer.hh"
#include "NetworkUtilityFunctions.hh"

std::mutex m2;

MultilayerNetworkServer::MultilayerNetworkServer()
{
  mQueue = new std::queue<GeneticAlgorithmReply>;
  mProcessed = new std::queue<int>;
  receiverThread = NULL;
}

MultilayerNetworkServer::~MultilayerNetworkServer()
{
  if(receiverThread != NULL)
  {
    receiverThread->join();
    delete receiverThread;
  }
  delete mQueue;
  delete mProcessed;
}

void MultilayerNetworkServer::start()
{
  // std::cout<<"MultilayerServer started"<<std::endl;
  receiverThread = new std::thread(&MultilayerNetworkServer::receiver, this);
}

void MultilayerNetworkServer::receiver()
{
  // std::cout<<"MultilayerServer receiver"<<std::endl;
  int argc;
  char **argv = NULL;
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;
  // boost::mpi::request req;
  GeneticAlgorithmReply inMessage;
  boost::mpi::status status;

  while(true)
  {
    // req = world.irecv(boost::mpi::any_source, boost::mpi::any_tag, inMessage);
    // status = req.wait();
    status = world.recv(boost::mpi::any_source, boost::mpi::any_tag, inMessage);
    if(status.tag() == 1)
    {
      return;
    }
    else if(status.tag() == 0)
    {
      m2.lock();
      mQueue->push(inMessage);
      // std::cout<<" +multinet got "<<inMessage.getNodeId()<<" -- size="<<mQueue->size()<<std::endl;
      m2.unlock();
    }
  }
}

void MultilayerNetworkServer::processQueue(MultilayerNetwork *multilayerNetwork)
{
  mNumberOfNodesToProcess = calculateNumberOfNodesToProcess(multilayerNetwork);
  GeneticAlgorithmReply inMessage;
  // std::cout<<"MultilayerServer processQueue"<<std::endl;
  while(true)
  {
    // std::cout<<"something starts"<<std::endl;
    m2.lock();
    if(finished() == true)
    {
      int argc;
      char **argv = NULL;
      boost::mpi::environment env{argc, argv};
      boost::mpi::communicator world;
      GeneticAlgorithmMessage tmpMessage(10);
      // boost::mpi::request req = world.isend(0, 1, tmpMessage);
      // req.wait();
      world.send(1, 1, tmpMessage);
      m2.unlock();
      return;
    }
    else if(mQueue->size() > 0)
    {
      inMessage = mQueue->front();
      int nodeId = inMessage.mNodeId;

      SerializedNetwork serializedNetwork = inMessage.mNetwork;
      Network* network = new Network;
      deserializeNetwork(&serializedNetwork, network);

      Node* node = multilayerNetwork->mNodesMap[nodeId];
      Network* networkAssigned = node->getNetworkAssigned();
      copyNetwork(network, networkAssigned);

      delete network;

      mQueue->pop();
      mProcessed->push(nodeId);
      // std::cout<<"multinet processed "<<nodeId<<std::endl;
    }
    m2.unlock();
  }
}

bool MultilayerNetworkServer::finished()
{
  if((int)mProcessed->size() >= mNumberOfNodesToProcess)
  {
    return true;
  }
  else
  {
    return false;
  }
}

int MultilayerNetworkServer::calculateNumberOfNodesToProcess(MultilayerNetwork* multilayerNetwork)
{
  int counter = 0;
  for(std::vector<int>::iterator itId=multilayerNetwork->mNodeIds.begin(); itId != multilayerNetwork->mNodeIds.end(); ++itId)
  {
    Node* currentNode = multilayerNetwork->mNodesMap[*itId];
    if(currentNode->getNetworkAssigned() != NULL)
    {
      ++counter;
    }
  }

  return counter;
}
