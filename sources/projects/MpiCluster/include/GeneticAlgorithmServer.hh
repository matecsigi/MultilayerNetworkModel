#ifndef GENETICALGORITHMSERVER_HH
#define GENETICALGORITHMSERVER_HH

#include <thread>
#include <queue>
#include "SerializedClasses.hh"

class GeneticAlgorithmServer
{
public:
  GeneticAlgorithmServer();
  ~GeneticAlgorithmServer();

  void start();
  void receiver();
  void processQueue();

  std::queue<GeneticAlgorithmMessage>* mQueue;
  std::thread* receiverThread;

  int mNumberOfRequests;
  int mNumberOfReceived;
  int mNumberOfProcessed;
};

#endif
