#include "ExecuteHebbian.hh"
#include <fstream>
#include <iostream>
#include <functional>
#include "GeneticAlgorithmController.hh"
#include "GlobalVariables.hh"
#include "HebbianFitnessFunction.hh"
#include "GeneralNetworkGenerator.hh"
#include "NetworkDynamicsGenerators.hh"
#include "NetworkInitialConditionGenerators.hh"
#include "IGeneticObserver.hh"
#include "GeneticObserver.hh"
#include "HebbianParameterContainer.hh"
#include "BarabasiModel.hh"

using namespace std::placeholders;

void executeHebbian(bool cluster, int argc, char*argv[])
{
  std::cout<<"Hello HebbianProject!"<<std::endl;

  HebbianParameterContainer *hebbianParameters = new HebbianParameterContainer;
  hebbianParameters->cluster = cluster;

  std::function<void(Network*)> binderBarabasiStructure = std::bind(generateNetwork, _1, hebbianParameters->higherNetworkSize, 3);

  std::function<void(Network*)> binderLinearDynamics = std::bind(linearNetworkDynamicsGenerator, _1);
  std::function<void(Network*)> binderInitialConditions = std::bind(oneNetworkInitialConditions, _1);
  std::function<void(Network*, int)> binderGeneral = std::bind(generateNetworkGeneral, _1, _2, binderBarabasiStructure, binderLinearDynamics, binderInitialConditions);

  GeneticAlgorithmParameterContainer *geneticParameters = new GeneticAlgorithmParameterContainer();
  geneticParameters->defaultCall = 1;
  geneticParameters->numberOfGenerations = 10000;
  geneticParameters->initialPopulationSize = 30;
  geneticParameters->modificationTypeProbabilities = vectorReconfModTypeProbabilities;
  geneticParameters->fitnessFunction = std::bind(hebbianFitnessFunction, _1, hebbianParameters);

  if(argc > 1)
  {
    std::string populationFolder(argv[1]);
    std::function<void(Network*, int)> binderLoadPopulation = std::bind(loadPopulation, _1, _2, populationFolder);
    geneticParameters->createInitialNetwork = binderLoadPopulation;
  }
  else
  {
    geneticParameters->createInitialNetwork = binderGeneral;    
  }
 
  std::ofstream fileParam("bin/generated/parameters.txt");
  fileParam<<hebbianParameters->toString()<<std::endl;
  fileParam<<geneticParameters->toString();
  fileParam.close();

  GeneticAlgorithmController geneticController(geneticParameters);
  IGeneticObserver *observer = new GeneticObserver(&geneticController);

  // Network* network = new Network;
  // std::string filename = "bin/generated/start.json";
  // int counter = 1;
  // loadNetworkFromJSON(network, filename, counter);

  geneticController.runGeneticAlgorithm(NULL, observer);

  // delete network;
  delete hebbianParameters;
  delete geneticParameters;
  delete observer;
}
