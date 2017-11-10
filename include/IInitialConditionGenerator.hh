#ifndef IINITIALCONDITIONGENERATOR_HH
#define IINITIALCONDITIONGENERATOR_HH

#include "MultilayerNetwork.hh"

class IInitialConditionGenerator
{
public:
  IInitialConditionGenerator(void){};
  IInitialConditionGenerator(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork = multilayerNetwork;};
  virtual ~IInitialConditionGenerator(void){};

  virtual void generateInitialCondition() = 0;
protected:
  MultilayerNetwork* mMultilayerNetwork;
};

#endif

