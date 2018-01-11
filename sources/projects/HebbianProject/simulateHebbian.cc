#include <iostream>
#include <functional>
#include "GeneticAlgorithmController.hh"
#include "GlobalVariables.hh"
#include "HebbianFitnessFunction.hh"
#include "GenerateBarabasiNetwork.hh"
#include "GeneralNetworkGenerator.hh"

using namespace std::placeholders;

int main()
{
  std::cout<<"Hello HebbianProject!"<<std::endl;
  std::function<void(Network*)> binderBarabasi = std::bind(generateBarabasiNetwork, _1, 30);
  std::function<void(Network*)> binderGeneral = std::bind(generateNetwork, _1, std::ref(binderBarabasi));
  GeneticAlgorithmController geneticController(vectorReconfModTypeProbabilities, hebbianFitnessFunction, binderGeneral);
  geneticController.runGeneticAlgorithm();

  return 0;
}
