#include <boost/mpi.hpp>
#include <thread>
#include <mutex>
#include "MultilayerNetworkServer.hh"

std::mutex m2;

MultilayerNetworkServer::MultilayerNetworkServer()
{
  mQueue = new std::queue<int>;
  mProcessed = new std::queue<int>;
  receiverThread = NULL;
}

MultilayerNetworkServer::~MultilayerNetworkServer()
{
  std::cout<<"server delete started"<<std::endl;
  delete mQueue;
  delete mProcessed;
  if(receiverThread != NULL)
  {
    receiverThread->join();
    delete receiverThread;
  }
  std::cout<<"server deleted"<<std::endl;
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
  int i;
  boost::mpi::status status;

  while(true)
  {
    status = world.recv(boost::mpi::any_source, boost::mpi::any_tag, i);
    if(status.tag() == 1)
    {
      return;
    }
    else if(status.tag() == 0)
    {
      m2.lock();
      mQueue->push(i);
      std::cout<<"multinet got "<<i<<" -- size="<<mQueue->size()<<std::endl;
      m2.unlock();
    }
  }
}

void MultilayerNetworkServer::processQueue(MultilayerNetwork *multilayerNetwork)
{
  int i;
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
      world.send(0, 1, i);
      m2.unlock();
      return;
    }
    else if(mQueue->size() > 0)
    {
      i = mQueue->front();
      mQueue->pop();
      mProcessed->push(i);
      std::cout<<"multinet processed "<<i<<std::endl;
    }
    m2.unlock();
  }
}
