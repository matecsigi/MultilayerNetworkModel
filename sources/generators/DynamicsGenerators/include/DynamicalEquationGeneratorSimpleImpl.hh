#ifndef DYNAMICALEQUATIONGENERATORSIMPLEIMPL_HH
#define DYNAMICALEQUATIONGENERATORSIMPLEIMPL_HH

#include "IDynamicalEquationGenerator.hh"

class MultilayerNetwork;

class DynamicalEquationGeneratorSimpleImpl: public IDynamicalEquationGenerator
{
public:
  DynamicalEquationGeneratorSimpleImpl(MultilayerNetwork* multilayerNetwork){mMultilayerNetwork=multilayerNetwork;};
  ~DynamicalEquationGeneratorSimpleImpl(){};

  void generateDynamicalEquations();
};

#endif
