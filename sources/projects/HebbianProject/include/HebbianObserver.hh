#ifndef HEBBIANOBSERVER_HH
#define HEBBIANOBSERVER_HH

#include "IObserver.hh"
#include <vector>

class MultilayerNetwork;
class Network;

class HebbianObserver: public IObserver
{
public:
  HebbianObserver(MultilayerNetwork* multilayerNetwork):IObserver(multilayerNetwork){mMultilayerNetwork = multilayerNetwork;};
  ~HebbianObserver(void);

  void atStart();
  void atStep();
  void atFinish();
  double getResult();

private:
  std::vector<Network*> mLowerNetworks;
  std::vector<Network*> mHebbianNetworks;
  std::vector<double> mDistances;

  void evaluateHebbianLearning(Network* referenceNetwork, Network* network);
};

#endif
