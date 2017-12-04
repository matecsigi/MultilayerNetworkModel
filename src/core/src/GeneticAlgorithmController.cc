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
  mTargetVectorField = targetVectorField;

  createInitialPopulation(network);
  for(mGeneration=1; mGeneration<numberOfGenerations; ++mGeneration)
  {
    mutation();
    crossover();
    death();
    // chooseBestNetwork();
  }

  Network* bestNetwork = chooseBestNetwork();
  // std::cout<<"END"<<std::endl;
  NetworkModifier networkModifier;
  networkModifier.copyNetwork(bestNetwork, network);
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
  for(int i=0; i<numberOfCrossovers; ++i)
  {
    NetworkPopulationElement* networkElement1 = chooseForCrossover();
    NetworkPopulationElement* networkElement2 = chooseForCrossover();
    Network* parentNetwork1 = networkElement1->getNetwork();
    Network* parentNetwork2 = networkElement2->getNetwork();
    Network* childNetwork = new Network;
    createMixedNetwork(parentNetwork1, parentNetwork2, childNetwork);

    NetworkPopulationElement* childElement = new NetworkPopulationElement(childNetwork, mTargetVectorField);
    childElement->setGeneration(mGeneration);
    mPopulation.push_back(childElement);
  }
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

void GeneticAlgorithmController::createInitialPopulation(Network* network)
{
  NetworkModifier networkModifier;
  
  for(int i=0; i<initialPopulationSize; ++i)
  {
    Network* networkModified = new Network;
    networkModifier.copyNetwork(network, networkModified);
    networkModifier.modifyNetwork(networkModified);
    NetworkPopulationElement* populationElement = new NetworkPopulationElement(networkModified, mTargetVectorField);
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

//Helpers

void GeneticAlgorithmController::createMixedNetwork(Network* parentNetwork1, Network* parentNetwork2, Network* childNetwork)
{
  std::vector<Node*> nodes = parentNetwork1->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* parentNode = (*itNode);
    childNetwork->addNode(parentNode->getId());

    Node* childNode = childNetwork->getNodeById(parentNode->getId());
    double* tmpBuffer = new double[bufferSize];
    parentNode->getValues(tmpBuffer);
    childNode->setValues(tmpBuffer);
    delete [] tmpBuffer;
  }

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Network* parentNetwork;
    Node* parentNode;
    double random = rand()/RAND_MAX;
    if(random < 0.5)
    {
      parentNetwork = parentNetwork1;
      parentNode = parentNetwork1->getNodeById((*itNode)->getId());
    }
    else
    {
      parentNetwork = parentNetwork2;
      parentNode = parentNetwork2->getNodeById((*itNode)->getId());      
    }
    Node* childNode = childNetwork->getNodeById((*itNode)->getId());

    std::vector<Node*> neighbors = parentNetwork->getNodeNeighbors(parentNode->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      Node* parentNeighbor = (*itNei);
      int localId1 = childNetwork->getLocalId(parentNode->getId());
      int localId2 = childNetwork->getLocalId(parentNeighbor->getId());
      childNetwork->addEdge(localId1, localId2);
    }

    std::string strEquation = parentNetwork->getNodeDynamicalEquationString(parentNode->getId());
    childNetwork->setDynamicalEquation(childNode->getId(), strEquation);
    DynamicalEquation* nodeEquation = childNetwork->getNodeDynamicalEquation(childNode->getId());
    std::vector<Node*> nodes = childNetwork->getNodeNeighbors(childNode->getId());
    std::map<int, Node*> nodesMap;
    for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
    {
      nodesMap[(*itNode)->getId()] = *itNode;
    }
    nodeEquation->loadNodesToEquation(nodeEquation->getBaseCalculationNode(), nodesMap);
  } 
}

Network* GeneticAlgorithmController::chooseBestNetwork()
{
  NetworkPopulationElement* bestPopulationElement = mPopulation[0];
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    NetworkPopulationElement* currentElement = (*itNet);
    if(currentElement->getFitness() > bestPopulationElement->getFitness())
    {
      bestPopulationElement = currentElement;
    }
  }
  std::cout<<"fitness="<<bestPopulationElement->getFitness()<<std::endl;
  return bestPopulationElement->getNetwork();
}
