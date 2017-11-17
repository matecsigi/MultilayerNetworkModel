#ifndef STRUCTUREGENERATORIMPL_HH
#define STRUCTUREGENERATORIMPL_HH

#include "IStructureGenerator.hh"

class StructureGeneratorImpl: public IStructureGenerator
{
public:
  StructureGeneratorImpl(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork=multilayerNetwork;};
  ~StructureGeneratorImpl(){};

  void generateStructure();
private:
  void generateLayers(void);
  void generateNetworks(void);
  void generateNodes(void);
  void assignNetworksToNodes(void);
};

#endif
