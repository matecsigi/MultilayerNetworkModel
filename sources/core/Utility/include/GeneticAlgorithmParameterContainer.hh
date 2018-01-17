#ifndef GENETICALGORITHMPARAMETERCONTAINER_HH
#define GENETICALGORITHMPARAMETERCONTAINER_HH

#include "NetworkPopulationElement.hh"
#include "GlobalVariables.hh"
#include <functional>

class GeneticAlgorithmParameterContainer
{
public:
  GeneticAlgorithmParameterContainer(){};
  ~GeneticAlgorithmParameterContainer(){};

  int initialPopulationSize = 10; //250
  int numberOfGenerations = 5; //50
  double mutationRatio = 0.15;
  double crossoverRatio = 0.15;
  double deathRatio = 0.3;
  /**
     The highest rankded elements are not deleted.
     This strategy is called elitism.
  */
  double elitRatio = 0.5;

  std::vector<double> modificationTypeProbabilities = vectorReconfModTypeProbabilities;

  double (*fitnessFunction)(NetworkPopulationElement*) = NULL;
  std::function<void (Network*)> createInitialNetwork = nullptr;
};

#endif
