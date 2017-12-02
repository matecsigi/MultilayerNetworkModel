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
  std::cout<<"mutation="<<numberOfMutations<<std::endl;
  NetworkPopulationElement* networkElement = chooseForMutation();
  std::cout<<networkElement<<std::endl;
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
  std::cout<<"death="<<numberOfDeaths<<std::endl;
  NetworkPopulationElement* networkElement = chooseForDeath();
  std::cout<<networkElement<<std::endl;
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
