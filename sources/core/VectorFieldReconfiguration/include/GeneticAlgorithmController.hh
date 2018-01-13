#ifndef GENETICALGORITHMCONTROLLER_HH
#define GENETICALGORITHMCONTORLLER_HH

#include "NetworkModifier.hh"
#include "NetworkPopulationElement.hh"
#include "IGeneticObserver.hh"
#include <functional>

class GeneticAlgorithmController
{
public:
  GeneticAlgorithmController(std::vector<double> &modificationTypeProbabilities = vectorReconfModTypeProbabilities, double (*fitnessFunction)(NetworkPopulationElement*) = NULL, std::function<void (Network*)> createInitialNetwork = nullptr);
  ~GeneticAlgorithmController();

  void runGeneticAlgorithm(Network* network = NULL, IGeneticObserver *observer = NULL);

  void fitToVectorField(Network* network, VectorField* targetVectorField);

  void mutation();
  void crossover();
  void death();
  void createInitialPopulation();

  friend void createInitialNetworkByModification(Network* network, Network* referenceNetwork, GeneticAlgorithmController *geneticController);

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
  Network* mNetwork;
  int mGeneration;
  VectorField* mTargetVectorField;
  std::vector<NetworkPopulationElement*> mPopulation;
  std::vector<double> mModificationTypeProbabilities;
  double (*mFitnessFunction)(NetworkPopulationElement*);
  // void (*mCreateInitialNetwork)(Network* network, Network* referenceNetwork, GeneticAlgorithmController *geneticController);
  // void (*mCreateInitialNetwork)(Network* network);
  std::function<void (Network*)> mCreateInitialNetwork;
};

#endif
