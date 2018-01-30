#include "GeneticObserver.hh"
#include "UtilityFunctions.hh"
#include "GeneticAlgorithmController.hh"
#include <iostream>
#include <spdlog/spdlog.h>

GeneticObserver::~GeneticObserver(void)
{
}

void GeneticObserver::atStart()
{
  std::cout<<"at start"<<std::endl;
  auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
  logger->info("Genetic algorithm started");
  spdlog::drop_all();
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

  auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
  logger->info("generation={}", mGeneticAlgorithmController->getGeneration());
  logger->info("  best={}", bestNetwork->getFitness());
  logger->info("  avg={}", mGeneticAlgorithmController->calculateAverageFitness());
  spdlog::drop_all();

  std::cout<<"========================="<<std::endl;
  std::cout<<"generation="<<mGeneticAlgorithmController->getGeneration()<<std::endl;
  std::cout<<"  ->BEST="<<bestNetwork->getFitness()<<std::endl;
  std::cout<<"  ->AVG="<<mGeneticAlgorithmController->calculateAverageFitness()<<std::endl;
  // mGeneticAlgorithmController->printPopulation();
}

void GeneticObserver::atFinish()
{

}
