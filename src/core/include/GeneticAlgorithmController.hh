#ifndef GENETICALGORITHMCONTROLLER_HH
#define GENETICALGORITHMCONTORLLER_HH

#include "NetworkModifier.hh"
#include "NetworkPopulationElement.hh"

class GeneticAlgorithmController
{
public:
  GeneticAlgorithmController();
  ~GeneticAlgorithmController();

  void fitToVectorField(Network* network, VectorField* targetVectorField);

  void mutation();
  void crossover();
  void death();
  void createInitialPopulation(Network* network, VectorField* targetVectorField);

private:
std::vector<NetworkPopulationElement*> mPopulation;
};

#endif
