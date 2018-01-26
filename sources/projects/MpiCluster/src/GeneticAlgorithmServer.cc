#include <boost/mpi.hpp>
#include "GeneticAlgorithmServer.hh"
#include <iostream>
#include <mutex>

std::mutex m;

GeneticAlgorithmServer::GeneticAlgorithmServer()
{
  mQueue = new std::queue<int>;
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
  int i;

  while(true)
  {
    world.recv(0, 0, i);
    m.lock();
    mQueue->push(i);
    std::cout<<"genetic received "<<i<<" -- size="<<mQueue->size()<<std::endl;
    m.unlock();
  }
}

void GeneticAlgorithmServer::processQueue()
{
  int argc;
  char **argv = NULL;
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;
  int i;

  std::cout<<"GeneticServer processQueue"<<std::endl;
  while(true)
  {
    m.lock();
    // std::cout<<"proc "<<mQueue->size()<<std::endl;
    if(mQueue->size() > 0)
    {
      i = mQueue->front();
      mQueue->pop();
      world.send(0, 0, i);
      std::cout<<"genetic send "<<i<<std::endl;
    }
    m.unlock();
  }
}

