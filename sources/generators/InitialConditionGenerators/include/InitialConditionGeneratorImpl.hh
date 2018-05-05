#ifndef INITIALCONDITIONGENERATORIMPL_HH
#define INITIALCONDITIONGENERATORIMPL_HH

#include "IInitialConditionGenerator.hh"
#include "GlobalVariables.hh"

class MultilayerNetwork;

class InitialConditionGeneratorImpl: public IInitialConditionGenerator
{
public:
  InitialConditionGeneratorImpl(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork=multilayerNetwork;};
  ~InitialConditionGeneratorImpl(){};

  void generateInitialCondition();
};

#endif
