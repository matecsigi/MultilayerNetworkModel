#include <boost/mpi.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;
  std::cout << world.rank() << ", " << world.size() << '\n';
}


// #include "mpi.h"
// #include <iostream>

// int main(int argc, char **argv)
// {
//     int rank, size, len;
//     char version[MPI_MAX_LIBRARY_VERSION_STRING];

//     MPI::Init();
//     rank = MPI::COMM_WORLD.Get_rank();
//     size = MPI::COMM_WORLD.Get_size();
//     MPI_Get_library_version(version, &len);
//     std::cout << "Hello, world!  I am " << rank << " of " << size
//               << "(" << version << ", " << len << ")" << std::endl;
//     MPI::Finalize();

//     return 0;
// }
