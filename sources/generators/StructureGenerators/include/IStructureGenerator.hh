#ifndef ISTRUCTUREGENERATOR_HH
#define ISTRUCTUREGENERATOR_HH

#include "MultilayerNetwork.hh"

class IStructureGenerator
{
public:
  IStructureGenerator(void){};
  IStructureGenerator(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork = multilayerNetwork;};
  virtual ~IStructureGenerator(void){};

  virtual void generateStructure(int numberOfNodesPerNetwork, int numberOfLayers) = 0;
protected:
  MultilayerNetwork* mMultilayerNetwork;
};

#endif
