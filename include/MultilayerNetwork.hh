#ifndef MULTILAYERNETWORK_HH
#define MULTILAYERNETWORK_HH

#include "Layer.hh"
#include <vector>

class MultilayerNetwork
{
public:
  MultilayerNetwork();
  ~MultilayerNetwork();

private:
  std::vector<Layer*> mLayers;
};
#endif
