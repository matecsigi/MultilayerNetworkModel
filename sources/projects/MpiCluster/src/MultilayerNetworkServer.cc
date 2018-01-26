#include <boost/mpi.hpp>
#include <thread>
#include <mutex>
#include "MultilayerNetworkServer.hh"

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
  std::cout<<"MultilayerServer started"<<std::endl;
  receiverThread = new std::thread(&MultilayerNetworkServer::receiver, this);
}

void MultilayerNetworkServer::receiver()
{
  std::cout<<"MultilayerServer receiver"<<std::endl;
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
    std::cout<<"ready to receive"<<std::endl;
    status = world.recv(boost::mpi::any_source, boost::mpi::any_tag, inMessage);
    std::cout<<" reccc "<<inMessage.getNodeId()<<std::endl;
    if(status.tag() == 1)
    {
      return;
    }
    else if(status.tag() == 0)
    {
      m2.lock();
      mQueue->push(inMessage);
      std::cout<<"multinet got "<<inMessage.getNodeId()<<" -- size="<<mQueue->size()<<std::endl;
      m2.unlock();
    }
  }
}

void MultilayerNetworkServer::processQueue(MultilayerNetwork *multilayerNetwork)
{
  GeneticAlgorithmReply inMessage;
  std::cout<<"MultilayerServer processQueue"<<std::endl;
  while(true)
  {
    m2.lock();
    if(mProcessed->size() >= 5)
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
      std::cout<<"initiate stop"<<std::endl;
      return;
    }
    else if(mQueue->size() > 0)
    {
      inMessage = mQueue->front();
      mQueue->pop();
      mProcessed->push(inMessage.getNodeId());
      std::cout<<"multinet processed "<<inMessage.getNodeId()<<std::endl;
    }
    m2.unlock();
  }
}
