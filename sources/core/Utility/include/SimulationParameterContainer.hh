#ifndef SIMULATIONPARAMETERCONTAINER_HH
#define SIMULATIONPARAMETERCONTAINER_HH

#include "GeneticAlgorithmParameterContainer.hh"

class SimulationParameterContainer
{
public:
  SimulationParameterContainer(){geneticParameters = new GeneticAlgorithmParameterContainer;};
  ~SimulationParameterContainer(){delete geneticParameters;};

  bool cluster = false;

  double odeTime = 0.5;
  double odeStepSize = 0.1;

  double downwardInfluenceMultiplier = 0.01;
  double upwardInfluenceMultiplier = 0.1;

  bool runVectorFieldReconfiguration = true;

  double adjustmentFactor = 0.3;
  std::function<double (double, double)> adjustmentDistanceFunction = [](double factor, double distance){return factor/distance;};

  GeneticAlgorithmParameterContainer *geneticParameters;
};

#endif
