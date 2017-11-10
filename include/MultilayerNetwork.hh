#ifndef MULTILAYERNETWORK_HH
#define MULTILAYERNETWORK_HH

#include "Node.hh"
#include "Layer.hh"
#include <vector>
#include <iostream>
#include <map>

class MultilayerNetwork
{
public:
  MultilayerNetwork();
  ~MultilayerNetwork();

  void addLayer(int layerId);
  std::vector<Layer*> getLayers(void) const;

  friend std::ostream& operator<<(std::ostream& os, const MultilayerNetwork& multilayerNetwork);
  friend bool operator==(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);
  friend bool initialConditionsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);
  
  void save(const char* filename);
  void load(const char* filename);
  
  void saveState(const char* filename);
  void loadState(const char* filename);
private:
  std::vector<Layer*> mLayers;

  void collectNodes(std::map<int, Node*>& nodesMap, std::vector<int>& nodeIds) const;
};
#endif
