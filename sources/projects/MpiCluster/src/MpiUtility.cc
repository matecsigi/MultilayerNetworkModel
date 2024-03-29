#include <boost/mpi.hpp>
#include "MpiUtility.hh"

void mpiSend(int destination, int tag, int value)
{
  int argc;
  char **argv = NULL;
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;

  world.send(destination, tag, value);
}
