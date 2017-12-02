#include "GeneticAlgorithmController.hh"
#include <algorithm>

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

  for(int i=0; i<numberOfGenerations; ++i)
  {
    mutation();
    crossover();
    death();
  }
  
}

void GeneticAlgorithmController::mutation()
{
  int numberOfMutations = mPopulation.size()*mutationRatio;
  for(int i=0; i<numberOfMutations; ++i)
  {
    NetworkPopulationElement* networkElement = chooseForMutation();
    Network* network = networkElement->getNetwork();
    NetworkModifier networkModifier;
    networkModifier.modifyNetwork(network);
    networkElement->setNetwork(network);
  }
}

void GeneticAlgorithmController::crossover()
{
  int numberOfCrossovers = mPopulation.size()*crossoverRatio;
  std::cout<<"crossover="<<numberOfCrossovers<<std::endl;
  NetworkPopulationElement* networkElement = chooseForCrossover();
  std::cout<<networkElement<<std::endl;
}

void GeneticAlgorithmController::death()
{
  int numberOfDeaths = mPopulation.size()*deathRatio;
  // std::cout<<"death="<<numberOfDeaths<<std::endl;
  for(int i=0; i<numberOfDeaths; ++i)
  {
    NetworkPopulationElement* networkElement = chooseForDeath();
    mPopulation.erase(std::remove(mPopulation.begin(), mPopulation.end(), networkElement), mPopulation.end());
  }
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

//Choosers

NetworkPopulationElement* GeneticAlgorithmController::chooseForMutation()
{
  int randomIndex = rand()%static_cast<int>(mPopulation.size());
  return mPopulation[randomIndex];
}

NetworkPopulationElement* GeneticAlgorithmController::chooseForCrossover()
{
  int randomIndex = rand()%static_cast<int>(mPopulation.size());
  return mPopulation[randomIndex];
}

NetworkPopulationElement* GeneticAlgorithmController::chooseForDeath()
{
  int randomIndex = rand()%static_cast<int>(mPopulation.size());
  return mPopulation[randomIndex];
}
