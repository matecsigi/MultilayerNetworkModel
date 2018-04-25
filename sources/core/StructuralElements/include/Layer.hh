#ifndef LAYER_HH
#define LAYER_HH

#include "Network.hh"
#include <vector>

class MultilayerNetwork;

class Layer
{
public:
  Layer();
  Layer(int);
  ~Layer();

  int getTime();

  void addNetwork(int networkId);
  Network* insertNetwork(Network* network);
  std::vector<Network*> getNetworks(void) const;

  int getId(void) const;

  void setMultilayerNetwork(MultilayerNetwork *multilayerNetwork);
  MultilayerNetwork* getMultilayerNetwork();

  void print();

  friend bool operator==(const Layer& layer1, const Layer& layer2);

private:
  int mLayerId;
  std::vector<Network*> mNetworks;
  MultilayerNetwork *mMultilayerNetwork;
};
#endif
