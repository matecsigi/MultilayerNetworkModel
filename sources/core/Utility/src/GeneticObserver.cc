#include "GeneticObserver.hh"
#include "UtilityFunctions.hh"
#include "GeneticAlgorithmController.hh"
#include <iostream>

GeneticObserver::~GeneticObserver(void)
{

}

void GeneticObserver::atStart()
{
  // std::cout<<"Genetic atStart"<<std::endl;
}

void GeneticObserver::atStep()
{
  // std::cout<<"Genetic atStep"<<std::endl;
  NetworkPopulationElement* bestNetwork = mGeneticAlgorithmController->chooseBestNetwork();

  // std::string filename = "sources/projects/HebbianProject/generated/geneticNetwork_gen";
  std::string filename = "bin/generated/hebbianNetworks/geneticNetwork_gen";
  int generation = mGeneticAlgorithmController->getGeneration();
  filename.append(std::to_string(generation));
  filename.append("_fitness");
  filename.append(std::to_string(bestNetwork->getFitness()));
  filename.append(".json");

  saveNetworkToJSON(bestNetwork->getNetwork(), filename);

  std::cout<<"========================="<<std::endl;
  std::cout<<"generation="<<mGeneticAlgorithmController->getGeneration()<<std::endl;
  std::cout<<"  ->BEST="<<bestNetwork->getFitness()<<std::endl;
  std::cout<<"  ->AVG="<<mGeneticAlgorithmController->calculateAverageFitness()<<std::endl;
  mGeneticAlgorithmController->printPopulation();
}

void GeneticObserver::atFinish()
{
  // std::cout<<"Genetic atFinish"<<std::endl;
}
