#ifndef SIMULATIONPARAMETERCONTAINER_HH
#define SIMULATIONPARAMETERCONTAINER_HH

#include "GeneticAlgorithmParameterContainer.hh"

class SimulationParameterContainer
{
public:
  SimulationParameterContainer(){geneticParameters = new GeneticAlgorithmParameterContainer;};
  ~SimulationParameterContainer(){delete geneticParameters;};

  double odeTime = 1;
  double odeStepSize = 0.1;

  GeneticAlgorithmParameterContainer *geneticParameters;
};

#endif
