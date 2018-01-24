#ifndef GENETICALGORITHMSERVER_HH
#define GENETICALGORITHMSERVER_HH

#include <queue>

class GeneticAlgorithmServer
{
public:
  GeneticAlgorithmServer();
  ~GeneticAlgorithmServer();

  void start();
  void receiver();
  void processQueue();

  std::queue<int> mQueue;
};

#endif
