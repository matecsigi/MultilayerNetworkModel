#ifndef ISTRUCTUREGENERATOR_HH
#define ISTRUCTUREGENERATOR_HH

#include "MultilayerNetwork.hh"

class IStructureGenerator
{
public:
  IStructureGenerator(void){};
  virtual ~IStructureGenerator(void){};

  virtual void generateStructure(MultilayerNetwork*) = 0;
};

#endif
