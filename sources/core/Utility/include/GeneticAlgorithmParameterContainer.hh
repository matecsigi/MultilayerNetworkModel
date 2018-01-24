#ifndef GENETICALGORITHMPARAMETERCONTAINER_HH
#define GENETICALGORITHMPARAMETERCONTAINER_HH

#include "NetworkPopulationElement.hh"
#include "GlobalVariables.hh"
#include <functional>
#include <string>

class GeneticAlgorithmParameterContainer
{
public:
  GeneticAlgorithmParameterContainer(){};
  ~GeneticAlgorithmParameterContainer(){};

  bool cluster = false;

  int defaultCall = 0;

  int initialPopulationSize = 100; //250
  int numberOfGenerations = 50; //50
  double mutationRatio = 0.15;
  double crossoverRatio = 0.15;
  double deathRatio = 0.3;
  /**
     The highest rankded elements are not deleted.
     This strategy is called elitism.
  */
  double elitRatio = 0.2;

  double maxConstantChange = 0.01;

  std::vector<double> modificationTypeProbabilities = vectorReconfModTypeProbabilities;

  std::function<double (NetworkPopulationElement*)> fitnessFunction = nullptr;
  std::function<void (Network*)> createInitialNetwork = nullptr;

  std::string toString();
};

#endif
