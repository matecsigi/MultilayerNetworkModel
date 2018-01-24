#include "ExecuteHebbian.hh"
#include <fstream>
#include <iostream>
#include <functional>
#include "GeneticAlgorithmController.hh"
#include "GlobalVariables.hh"
#include "HebbianFitnessFunction.hh"
#include "GenerateBarabasiNetwork.hh"
#include "GeneralNetworkGenerator.hh"
#include "NetworkDynamicsGenerators.hh"
#include "NetworkInitialConditionGenerators.hh"
#include "IGeneticObserver.hh"
#include "GeneticObserver.hh"
#include "HebbianParameterContainer.hh"

using namespace std::placeholders;

void executeHebbian(bool cluster)
{
  std::cout<<"Hello HebbianProject!"<<std::endl;

  HebbianParameterContainer *hebbianParameters = new HebbianParameterContainer;
  hebbianParameters->cluster = cluster;

  std::function<void(Network*)> binderBarabasiStructure = std::bind(generateBarabasiNetwork, _1, hebbianParameters->higherNetworkSize, 1);
  std::function<void(Network*)> binderLinearDynamics = std::bind(linearNetworkDynamicsGenerator, _1);
  std::function<void(Network*)> binderInitialConditions = std::bind(oneNetworkInitialConditions, _1);
  std::function<void(Network*)> binderGeneral = std::bind(generateNetwork, _1, binderBarabasiStructure, binderLinearDynamics, binderInitialConditions);

  GeneticAlgorithmParameterContainer *geneticParameters = new GeneticAlgorithmParameterContainer();
  geneticParameters->defaultCall = 1;
  geneticParameters->numberOfGenerations = 1000;
  geneticParameters->initialPopulationSize = 30;
  geneticParameters->modificationTypeProbabilities = vectorReconfModTypeProbabilities;
  geneticParameters->fitnessFunction = std::bind(hebbianFitnessFunction, _1, hebbianParameters);
  geneticParameters->createInitialNetwork = binderGeneral;

  std::ofstream fileParam("bin/generated/parameters.txt");
  fileParam<<hebbianParameters->toString()<<std::endl;
  fileParam<<geneticParameters->toString();
  fileParam.close();

  GeneticAlgorithmController geneticController(geneticParameters);
  IGeneticObserver *observer = new GeneticObserver(&geneticController);

  geneticController.runGeneticAlgorithm(NULL, observer);

  delete hebbianParameters;
  delete geneticParameters;
  delete observer;
}
