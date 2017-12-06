#ifndef STRUCTUREGENERATORBARABASIIMPL_HH
#define STRUCTUREGENERATORBARABASIIMPL_HH

#include "IStructureGenerator.hh"

class StructureGeneratorBarabasiImpl: public IStructureGenerator
{
public:
  StructureGeneratorBarabasiImpl(MultilayerNetwork* multilayerNetwork);
  ~StructureGeneratorBarabasiImpl(){};

  void generateStructure();

private:
  void addLayers();
  void addNetworks();
  void generateNetworks();

  std::string pythonBarabasiGenerator(int layerId, int networkId);

  int mNumberOfNodesOnTopLayer;
  int mNumberOfNodesInANetwork;
};

#endif
