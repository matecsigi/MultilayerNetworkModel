#ifndef MULTILAYERNETWORKSERVER_HH
#define MULTILAYERNETWORKSERVER_HH

#include "MultilayerNetwork.hh"
#include "SerializedClasses.hh"
#include <queue>
#include <thread>

class MultilayerNetworkServer
{
public:
  MultilayerNetworkServer();
  ~MultilayerNetworkServer();

  void start();
  void receiver();
  void processQueue(MultilayerNetwork* multilayerNetwork);
  bool finished();
  int calculateNumberOfNodesToProcess(MultilayerNetwork* multilayerNetwork);

  std::queue<GeneticAlgorithmReply> *mQueue;
  std::queue<int> *mProcessed;
  std::thread* receiverThread;
  int mNumberOfNodesToProcess;
};

#endif
