#ifndef IDYNAMICALEQUATIONGENERATOR_HH
#define IDYNAMICALEQUATIONGENERATOR_HH

#include "MultilayerNetwork.hh"

class IDynamicalEquationGenerator
{
public:
  IDynamicalEquationGenerator(void){};
  IDynamicalEquationGenerator(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork = multilayerNetwork;};
  virtual ~IDynamicalEquationGenerator(void){};

  virtual void generateDynamicalEquations() = 0;
protected:
  MultilayerNetwork* mMultilayerNetwork;
};

#endif
