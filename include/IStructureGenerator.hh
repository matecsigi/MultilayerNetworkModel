#ifndef ISTRUCTUREGENERATOR_HH
#define ISTRUCTUREGENERATOR_HH

#include "MultilayerNetwork.hh"

class IStructureGenerator
{
public:
  IStructureGenerator(void){};
  ~IStructureGenerator(void){};

  virtual MultilayerNetwork generateStructure() = 0;
};

#endif
