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

using namespace std::placeholders;

int main()
{
  std::cout<<"Hello HebbianProject!"<<std::endl;
  std::function<void(Network*)> binderBarabasiStructure = std::bind(generateBarabasiNetwork, _1, 6, 1);
  std::function<void(Network*)> binderLinearDynamics = std::bind(linearNetworkDynamicsGenerator, _1);
  std::function<void(Network*)> binderGeneral = std::bind(generateNetwork, _1, binderBarabasiStructure, binderLinearDynamics, nullptr);

  GeneticAlgorithmParameterContainer *geneticParameters = new GeneticAlgorithmParameterContainer();
  geneticParameters->modificationTypeProbabilities = vectorReconfModTypeProbabilities;
  geneticParameters->fitnessFunction = hebbianFitnessFunction;
  geneticParameters->createInitialNetwork = binderGeneral;
  GeneticAlgorithmController geneticController(geneticParameters);
  IGeneticObserver *observer = new GeneticObserver(&geneticController);
  geneticController.runGeneticAlgorithm(NULL, observer);

  delete geneticParameters;

  return 0;
}
