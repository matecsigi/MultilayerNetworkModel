#ifndef STRUCTUREGENERATORBARABASIIMPL_HH
#define STRUCTUREGENERATORBARABASIIMPL_HH

#include "IStructureGenerator.hh"

class StructureGeneratorBarabasiImpl: public IStructureGenerator
{
public:
  StructureGeneratorBarabasiImpl(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork=multilayerNetwork;};
  ~StructureGeneratorBarabasiImpl(){};

  void generateStructure();
};

#endif
