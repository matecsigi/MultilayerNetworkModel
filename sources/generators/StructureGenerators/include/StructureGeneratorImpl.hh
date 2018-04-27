#ifndef STRUCTUREGENERATORIMPL_HH
#define STRUCTUREGENERATORIMPL_HH

#include "IStructureGenerator.hh"

class StructureGeneratorImpl: public IStructureGenerator
{
public:
  StructureGeneratorImpl(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork=multilayerNetwork;};
  ~StructureGeneratorImpl(){};

  void generateStructure(int numberOfNodesPerNetwork, int numberOfLayers);
private:
  void recursiveGenerator(Node *node, int depth, int numberOfNodesPerNetwork, int numberOfLayers);
};

#endif
