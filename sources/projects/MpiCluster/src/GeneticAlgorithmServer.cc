#include <boost/mpi.hpp>
#include "GeneticAlgorithmServer.hh"
#include <iostream>
#include <thread>

GeneticAlgorithmServer::GeneticAlgorithmServer()
{

}

GeneticAlgorithmServer::~GeneticAlgorithmServer()
{

}

void GeneticAlgorithmServer::start()
{
  std::cout<<"GeneticAlgorithmServer started"<<std::endl;
  std::thread receiverThread = std::thread(&GeneticAlgorithmServer::receiver, this);
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
    mQueue.push(i);
    std::cout<<world.rank()<<" -> "<<mQueue.size()<<std::endl;
  }
}

void GeneticAlgorithmServer::processQueue()
{

}

