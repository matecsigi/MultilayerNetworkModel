#ifndef LAYER_HH
#define LAYER_HH

#include "Network.hh"
#include <vector>

class Layer
{
public:
  Layer();
  Layer(int);
  ~Layer();

  void addNetwork(int networkId);
  std::vector<Network*> getNetworks(void) const;

  int getId(void) const;

  friend bool operator==(const Layer& layer1, const Layer& layer2);

private:
  int mLayerId;
  std::vector<Network*> mNetworksInLayer;
};
#endif
