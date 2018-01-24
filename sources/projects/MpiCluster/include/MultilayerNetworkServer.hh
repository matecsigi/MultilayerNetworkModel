#ifndef MULTILAYERNETWORKSERVER_HH
#define MULTILAYERNETWORKSERVER_HH

#include "MultilayerNetwork.hh"
#include <queue>

class MultilayerNetworkServer
{
public:
  MultilayerNetworkServer();
  ~MultilayerNetworkServer();

  void start();
  void receiver();
  void processQueue(MultilayerNetwork* multilayerNetwork);

  std::queue<int> mQueue;
};

#endif
