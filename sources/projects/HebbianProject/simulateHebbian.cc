#include <iostream>
#include <functional>
#include "GeneticAlgorithmController.hh"
#include "GlobalVariables.hh"
#include "HebbianFitnessFunction.hh"
#include "GenerateBarabasiNetwork.hh"
#include "GeneralNetworkGenerator.hh"
#include "NetworkDynamicsGenerators.hh"
#include "IGeneticObserver.hh"
#include "GeneticObserver.hh"
#include "HebbianParameterContainer.hh"

using namespace std::placeholders;

int main()
{
  std::cout<<"Hello HebbianProject!"<<std::endl;

  HebbianParameterContainer *hebbianParameters = new HebbianParameterContainer;

  std::function<void(Network*)> binderBarabasiStructure = std::bind(generateBarabasiNetwork, _1, hebbianParameters->higherNetworkSize, 1);
  std::function<void(Network*)> binderLinearDynamics = std::bind(linearNetworkDynamicsGenerator, _1);
  std::function<void(Network*)> binderGeneral = std::bind(generateNetwork, _1, binderBarabasiStructure, binderLinearDynamics, nullptr);

  GeneticAlgorithmParameterContainer *geneticParameters = new GeneticAlgorithmParameterContainer();
  geneticParameters->numberOfGenerations = 1000;
  geneticParameters->initialPopulationSize = 25;
  geneticParameters->modificationTypeProbabilities = vectorReconfModTypeProbabilities;
  geneticParameters->fitnessFunction = std::bind(hebbianFitnessFunction, _1, hebbianParameters);
  geneticParameters->createInitialNetwork = binderGeneral;
  GeneticAlgorithmController geneticController(geneticParameters);
  IGeneticObserver *observer = new GeneticObserver(&geneticController);
  geneticController.runGeneticAlgorithm(NULL, observer);

  delete hebbianParameters;
  delete geneticParameters;
  delete observer;

  return 0;
}
