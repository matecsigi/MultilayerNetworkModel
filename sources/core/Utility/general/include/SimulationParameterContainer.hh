#ifndef SIMULATIONPARAMETERCONTAINER_HH
#define SIMULATIONPARAMETERCONTAINER_HH

#include "GeneticAlgorithmParameterContainer.hh"
#include "TraceParameterContainer.hh"
#include <algorithm>

class SimulationParameterContainer
{
public:
  SimulationParameterContainer(){geneticParameters = new GeneticAlgorithmParameterContainer; traceParameters = new TraceParameterContainer;};
  ~SimulationParameterContainer(){delete geneticParameters;};

  bool printTrace = false;

  bool cluster = false;

  double odeTime = 0.5;
  double odeStepSize = 0.1;

  double downwardInfluenceMultiplier = 0.01;
  double upwardInfluenceMultiplier = 0.1;

  bool runVectorFieldReconfiguration = true;

  double adjustmentFactor = 0.3;
  std::function<double (double, double)> adjustmentDistanceFunction = [](double factor, double distance){return std::min(1.0,factor/distance);};

  GeneticAlgorithmParameterContainer *geneticParameters;
  TraceParameterContainer *traceParameters;
};

#endif
