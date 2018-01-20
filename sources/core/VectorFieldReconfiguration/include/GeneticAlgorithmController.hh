#ifndef GENETICALGORITHMCONTROLLER_HH
#define GENETICALGORITHMCONTORLLER_HH

#include "NetworkModifier.hh"
#include "NetworkPopulationElement.hh"
#include "IGeneticObserver.hh"
#include "GeneticAlgorithmParameterContainer.hh"
#include <functional>

class GeneticAlgorithmController
{
public:
  GeneticAlgorithmController(GeneticAlgorithmParameterContainer *parameters = NULL);
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
  NetworkPopulationElement* chooseForDeath_helper();
  void createMixedNetwork(Network* parentNetwork1, Network* parentNetwork2, Network* childNetwork);
  NetworkPopulationElement* chooseBestNetwork();
  double calculateAverageFitness();
  void updateFitnessRanks();
  void quickSortTwoVectors(std::vector<double> &fitnessVector, int left, int right);
  int getGeneration();
  void printPopulation();

private:
  Network* mNetwork;
  int mGeneration;
  VectorField* mTargetVectorField;
  std::vector<NetworkPopulationElement*> mPopulation;

  //parameters
  GeneticAlgorithmParameterContainer mGeneticParameters;
  int mInitialPopulationSize;
  int mNumberOfGenerations;
  double mMutationRatio;
  double mCrossoverRatio;
  double mDeathRatio;
  double mElitRatio;
  std::vector<double> mModificationTypeProbabilities;
  std::function<double (NetworkPopulationElement*)> mFitnessFunction;
  std::function<void (Network*)> mCreateInitialNetwork;
};

#endif
