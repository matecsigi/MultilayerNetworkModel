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
  GeneticAlgorithmController geneticController(vectorReconfModTypeProbabilities, hebbianFitnessFunction, binderGeneral);
  IGeneticObserver *observer = new GeneticObserver(&geneticController);
  geneticController.runGeneticAlgorithm(NULL, observer);

  return 0;
}
