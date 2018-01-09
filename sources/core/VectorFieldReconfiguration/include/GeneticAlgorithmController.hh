#ifndef GENETICALGORITHMCONTROLLER_HH
#define GENETICALGORITHMCONTORLLER_HH

#include "NetworkModifier.hh"
#include "NetworkPopulationElement.hh"

class GeneticAlgorithmController
{
public:
  GeneticAlgorithmController(double (*fitnessFunction)(NetworkPopulationElement*) = NULL);
  ~GeneticAlgorithmController();

  void fitToVectorField(Network* network, VectorField* targetVectorField);

  void mutation();
  void crossover();
  void death();
  void createInitialPopulation(Network* network);

  //Choosers
  NetworkPopulationElement* chooseForMutation();
  NetworkPopulationElement* chooseForCrossover();
  NetworkPopulationElement* chooseForDeath();

  //Helper functions
  void createMixedNetwork(Network* parentNetwork1, Network* parentNetwork2, Network* childNetwork);
  Network* chooseBestNetwork();
  double calculateAverageFitness();
  void updateFitnessRanks();
  void quickSortTwoVectors(std::vector<double> &fitnessVector, int left, int right);

private:
  int mGeneration;
  VectorField* mTargetVectorField;
  std::vector<NetworkPopulationElement*> mPopulation;
  double (*mFitnessFunction)(NetworkPopulationElement*);
};

#endif
