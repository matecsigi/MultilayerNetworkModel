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

  int initialPopulationSize = 30; //250
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

  std::function<double (Network*)> fitnessFunction = nullptr;
  std::function<void (Network*, int)> createInitialNetwork = nullptr;

  /**
     When operating in a cluster the number of requests has to be sent in advance to the GeneticAlgorithmServers when they are using only one thread. The vector contains to number of requests. The index is the rank of the process.
   */
  std::vector<int> clusterMessageSizes;

  std::string toString();
};

#endif
