#ifndef INITIALCONDITIONGENERATORIMPL_HH
#define INITIALCONDITIONGENERATORIMPL_HH

#include "IInitialConditionGenerator.hh"

class InitialConditionGeneratorImpl: public IInitialConditionGenerator
{
public:
  InitialConditionGeneratorImpl(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork=multilayerNetwork;};
  ~InitialConditionGeneratorImpl(){};

  void generateInitialCondition();
private:
};

#endif
