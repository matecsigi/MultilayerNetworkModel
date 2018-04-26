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

  void addNetwork(int networkId);
  Network* insertNetwork(Network* network);
  std::vector<Network*> getNetworks(void) const;

  void setMultilayerNetwork(MultilayerNetwork *multilayerNetwork);
  MultilayerNetwork* getMultilayerNetwork();

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
