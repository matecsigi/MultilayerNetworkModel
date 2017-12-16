#include "GeneticAlgorithmController.hh"
#include <algorithm>
#include <stdlib.h>

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
  for(mGeneration=1; mGeneration<numberOfGenerations+1; ++mGeneration)
  {
    // std::cout<<"generation="<<mGeneration<<std::endl;
    mutation();
    crossover();
    death();
    chooseBestNetwork();
    // std::cout<<"  --avg="<<calculateAverageFitness()<<std::endl;
  }

  Network* bestNetwork = chooseBestNetwork();
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
    networkElement->setGeneration(mGeneration);
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

    // std::cout<<"    -parent="<<networkElement1->getFitness()<<", "<<networkElement2->getFitness()<<"->"<<childElement->getFitness()<<std::endl;
  }
}

void GeneticAlgorithmController::death()
{
  // int numberOfDeaths = mPopulation.size()*deathRatio;
  int numberOfDeaths = mPopulation.size()-initialPopulationSize;
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
    networkModifier.modifyNetwork(networkModified, 4);
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
  double sumFitness = 0;
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    // sumFitness += pow(2, (*itNet)->getFitness());
    sumFitness += (*itNet)->getFitness()*(*itNet)->getFitness();
  }

  double random = (static_cast<double>(rand())/RAND_MAX)*sumFitness;
  double counter = 0;
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    // counter += pow(2, (*itNet)->getFitness());
    counter += (*itNet)->getFitness()*(*itNet)->getFitness();
    if(counter >= random)
    {
      return (*itNet);
    }
  }

  return mPopulation[0];
  // int randomIndex = rand()%static_cast<int>(mPopulation.size());
  // return mPopulation[randomIndex];
}

NetworkPopulationElement* GeneticAlgorithmController::chooseForDeath()
{
  for(int i=2; i>=0; --i)
  {
    for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
    {
      int generation = (*itNet)->getGeneration();
      if(generation <= mGeneration-i)
      {
  	return (*itNet);
      }
    }
  }

  int randomIndex = rand()%static_cast<int>(mPopulation.size());
  return mPopulation[randomIndex];
}

//Helpers

void GeneticAlgorithmController::createMixedNetwork(Network* parentNetwork1, Network* parentNetwork2, Network* childNetwork)
{
  std::vector<Node*> nodes = parentNetwork1->getNodes();
  double* tmpBuffer = new double[bufferSize];

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* parentNode = (*itNode);
    childNetwork->addNode(parentNode->getId());

    Node* childNode = childNetwork->getNodeById(parentNode->getId());
    parentNode->getValues(tmpBuffer);
    childNode->setValues(tmpBuffer);
  }
  delete [] tmpBuffer;

  struct random_data* rand_state;
  char* rand_statebufs;
  rand_statebufs = (char*)calloc(1, 32);
  rand_state = (struct random_data*)calloc(1, sizeof(struct random_data));
  initstate_r(random(), rand_statebufs, 32, rand_state);

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Network* parentNetwork;
    Node* parentNode;
    // double random = static_cast<double>(rand())/RAND_MAX;

    int randInt;
    random_r((struct random_data*)rand_state, &randInt);
    double random = static_cast<double>(randInt)/RAND_MAX;

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
      int nodeId1 = parentNode->getId();
      int nodeId2 = parentNeighbor->getId();
      childNetwork->addEdge(nodeId1, nodeId2);
    }

    // std::string strEquation = parentNetwork->getNodeDynamicalEquationString(parentNode->getId());
    // childNetwork->setDynamicalEquation(childNode->getId(), strEquation);

    DynamicalEquation* dynamicalEquation = parentNetwork->getNodeDynamicalEquation(parentNode->getId());
    childNetwork->setDynamicalEquation(childNode->getId(), dynamicalEquation);

    DynamicalEquation* nodeEquation = childNetwork->getNodeDynamicalEquation(childNode->getId());
    std::vector<Node*> nodes = childNetwork->getNodeNeighbors(childNode->getId());
    nodes.push_back(childNode);
    std::map<int, Node*> nodesMap;
    for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
    {
      nodesMap[(*itNode)->getId()] = *itNode;
    }
    nodeEquation->loadNodesToEquation(nodeEquation->getBaseCalculationNode(), nodesMap);
  }
  free(rand_state);
  free(rand_statebufs);
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
  // std::cout<<"  -fitness="<<bestPopulationElement->getFitness()<<std::endl;
  return bestPopulationElement->getNetwork();
}

double GeneticAlgorithmController::calculateAverageFitness()
{
  double sumFitness = 0;
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    sumFitness += (*itNet)->getFitness();
  }

  return sumFitness/mPopulation.size();
}
