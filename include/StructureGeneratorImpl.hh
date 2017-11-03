#ifndef STRUCTUREGENERATOR_HH
#define STRUCTUREGENERATOR_HH

#include "IStructureGenerator.hh"

class StructureGeneratorImpl: public IStructureGenerator
{
public:
  MultilayerNetwork generateStructure(void);
};

#endif
