#ifndef HEBBIANOBSERVER_HH
#define HEBBIANOBSERVER_HH

#include "IObserver.hh"
#include "Network.hh"
#include "MultilayerNetwork.hh"
#include <vector>

class HebbianObserver: public IObserver
{
public:
  HebbianObserver(MultilayerNetwork* multilayerNetwork):IObserver(multilayerNetwork){mMultilayerNetwork = multilayerNetwork;};
  ~HebbianObserver(void);

  void atStart();
  void atStep();
  void atFinish();

private:
  std::vector<Network*> mLowerNetworks;
  std::vector<Network*> mHebbianNetworks;

  void evaluateHebbianLearning(Network* referenceNetwork, Network* network);
};

#endif
