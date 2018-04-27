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

  //------------------------------------
  //-----Basic--------------------------
  //------------------------------------
  int getId(void) const;

  int getTime();

  Network* addNetwork();
  void addNetworkById(int networkId);
  Network* insertNetwork(Network* network);
  std::vector<Network*> getNetworks(void) const;

  MultilayerNetwork* getMultilayerNetwork();
  void setMultilayerNetwork(MultilayerNetwork *multilayerNetwork);

  //------------------------------------
  //-----Utility------------------------
  //------------------------------------

  void print();

private:
  int mLayerId;
  std::vector<Network*> mNetworks;
  MultilayerNetwork *mMultilayerNetwork;
};
#endif
