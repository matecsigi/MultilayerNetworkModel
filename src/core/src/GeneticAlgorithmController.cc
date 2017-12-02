#include "GeneticAlgorithmController.hh"

GeneticAlgorithmController::GeneticAlgorithmController()
{

}

GeneticAlgorithmController::~GeneticAlgorithmController()
{
  for(std::vector<NetworkPopulationElement*>::iterator itPop=mPopulation.begin(); itPop != mPopulation.end(); ++itPop)
  {
    delete (*itPop);
  }
}

void GeneticAlgorithmController::fitToVectorField(Network* network, VectorField* targetVectorField)
{
  createInitialPopulation(network, targetVectorField);

  // for(int i=0; i<numberOfGenerations; ++i)
  // {
  //   mutation(population);
  //   crossover(population);
  //   death(population);
  // }

  
}

void GeneticAlgorithmController::createInitialPopulation(Network* network, VectorField* targetVectorField)
{
  NetworkModifier networkModifier;
  
  for(int i=0; i<initialPopulationSize; ++i)
  {
    Network* networkModified = new Network;
    networkModifier.copyNetwork(network, networkModified);
    networkModifier.modifyNetwork(networkModified);
    NetworkPopulationElement* populationElement = new NetworkPopulationElement(networkModified, targetVectorField);
    populationElement->setGeneration(0);
    mPopulation.push_back(populationElement);
  }
}
