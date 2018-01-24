#include <boost/mpi.hpp>
#include <iostream>
#include "ExecuteHebbian.hh"
#include "GeneticAlgorithmServer.hh"

int main(int argc, char *argv[])
{
  std::cout<<"Hello Cluster!"<<std::endl;
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;

  if(world.rank() == 0)
  {
    executeHebbian(true);
  }
  else if(world.rank() != 0)
  {
    GeneticAlgorithmServer geneticAlgorithmServer;
    geneticAlgorithmServer.startServer();
  }
}
