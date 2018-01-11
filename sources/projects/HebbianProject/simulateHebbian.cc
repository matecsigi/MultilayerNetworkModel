#include <iostream>
#include <functional>
#include "GeneticAlgorithmController.hh"
#include "GlobalVariables.hh"
#include "HebbianFitnessFunction.hh"
#include "GenerateBarabasiNetwork.hh"

using namespace std::placeholders;

int main()
{
  std::cout<<"Hello HebbianProject!"<<std::endl;
  GeneticAlgorithmController geneticController(vectorReconfModTypeProbabilities, hebbianFitnessFunction, std::bind(generateBarabasiNetwork, _1, 30));
  geneticController.runGeneticAlgorithm();

  return 0;
}
