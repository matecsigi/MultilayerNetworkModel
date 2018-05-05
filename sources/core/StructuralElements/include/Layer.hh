#ifndef LAYER_HH
#define LAYER_HH

#include <vector>
#include <map>

class MultilayerNetwork;
class Network;

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
  Network* insertNetwork(Network* network, std::map<int,int>* idMap = NULL);
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
