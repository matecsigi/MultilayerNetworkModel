#ifndef MULTILAYERNETWORK_HH
#define MULTILAYERNETWORK_HH

#include "Layer.hh"
#include <vector>

class MultilayerNetwork
{
public:
  MultilayerNetwork();
  ~MultilayerNetwork();

  void addLayer(int);

  std::vector<Layer*> getLayers(void);

private:
  std::vector<Layer*> mLayers;
};
#endif
