#include "GeneticObserver.hh"
#include "Network.hh"
#include "UtilityFunctions.hh"
#include "GeneticAlgorithmController.hh"
#include <iostream>
#include <spdlog/spdlog.h>
#include <sys/stat.h>

GeneticObserver::~GeneticObserver(void)
{
}

void GeneticObserver::atStart()
{
  createDirectory("bin/generated");
  createDirectory("bin/generated/hebbianNetworks");

  std::cout<<"at start"<<std::endl;
  auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
  logger->info("Genetic algorithm started");
  spdlog::drop_all();
}

void GeneticObserver::atStep()
{
  // NetworkPopulationElement* bestNetwork = mGeneticAlgorithmController->chooseBestNetwork();
  // std::string filename = "bin/generated/hebbianNetworks/geneticNetwork_gen";
  // int generation = mGeneticAlgorithmController->getGeneration();
  // filename.append(std::to_string(generation));
  // filename.append("_fitness");
  // filename.append(std::to_string(bestNetwork->getFitness()));
  // filename.append(".json");

  //saveNetworkToJSON(bestNetwork->getNetwork(), filename);

  NetworkPopulationElement* bestNetwork = mGeneticAlgorithmController->chooseBestNetwork();

  auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
  logger->info("generation={}", mGeneticAlgorithmController->getGeneration());
  logger->info("best={} fitness={}", bestNetwork->getNetwork()->getId(), bestNetwork->getFitness());
  logger->info("avg={}", mGeneticAlgorithmController->calculateAverageFitness());

  logger->info("population");
  int generation = mGeneticAlgorithmController->getGeneration();
  std::vector<NetworkPopulationElement*> population = mGeneticAlgorithmController->getPopulation();
  for(std::vector<NetworkPopulationElement*>::iterator itPop=population.begin(); itPop != population.end(); ++itPop)
  {
    NetworkPopulationElement* currentElement = (*itPop);
    
    std::string folderName = "bin/generated/hebbianNetworks/gen-";
    folderName.append(std::to_string(generation));
    mkdir(folderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    std::string fileName = folderName;
    fileName.append("/network-");
    fileName.append(std::to_string(currentElement->getNetwork()->getId()));
    fileName.append("-fitness-");
    fileName.append(std::to_string(currentElement->getFitness()));
    fileName.append(".json");

    saveNetworkToJSON(currentElement->getNetwork(), fileName);
    
    logger->info("id={} fitness={}", currentElement->getNetwork()->getId(), currentElement->getFitness());
  }

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
