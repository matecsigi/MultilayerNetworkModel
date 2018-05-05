#include "GeneticAlgorithmController.hh"
#include "Network.hh"
#include "Node.hh"
#include "NetworkUtilityFunctions.hh"
#include "NetworkClassUtility.hh"
#include <algorithm>
#include <stdlib.h>
#include <ctime>
#include <chrono>
#include <spdlog/spdlog.h>

using namespace std::placeholders;

void createInitialNetworkByModification(Network* network, int control, Network* referenceNetwork, GeneticAlgorithmController *geneticController)
{
  NetworkModifier networkModifier(&geneticController->mGeneticParameters);
  copyNetwork(referenceNetwork, network);
  networkModifier.modifyNetwork(network, 25);
}

GeneticAlgorithmController::GeneticAlgorithmController(GeneticAlgorithmParameterContainer *parameters)
{
  bool deletionNeeded = false;
  if(parameters == NULL){parameters = new GeneticAlgorithmParameterContainer; deletionNeeded = true;};

  mGeneticParameters = *parameters;

  mNetwork = NULL;
  mDefaultCall = parameters->defaultCall;
  mInitialPopulationSize = parameters->initialPopulationSize;
  mNumberOfGenerations = parameters->numberOfGenerations;
  mMutationRatio = parameters->mutationRatio;
  mCrossoverRatio = parameters->crossoverRatio;
  mDeathRatio = parameters->deathRatio;
  mElitRatio = parameters->elitRatio;
  mModificationTypeProbabilities = parameters->modificationTypeProbabilities;
  mFitnessFunction = parameters->fitnessFunction;
  mCreateInitialNetwork = parameters->createInitialNetwork;

  if(deletionNeeded == true){delete parameters;}
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
  mNetwork = network;
  if(mCreateInitialNetwork == nullptr)
  {
    mCreateInitialNetwork = std::bind(createInitialNetworkByModification, _1, _2, mNetwork, this);
  }  
  mTargetVectorField = targetVectorField;
  runGeneticAlgorithm(network);
}

void GeneticAlgorithmController::runGeneticAlgorithm(Network* network, IGeneticObserver *observer)
{
  mNetwork = network;
  if(mCreateInitialNetwork == nullptr)
  {
    mCreateInitialNetwork = std::bind(createInitialNetworkByModification, _1, _2, mNetwork, this);
  }

  if(observer != NULL){observer->atStart();}
  createInitialPopulation();
  for(mGeneration=1; mGeneration<mNumberOfGenerations+1; ++mGeneration)
  {
    // std::cout<<"generation="<<mGeneration<<std::endl;
    mutation();
    crossover();
    death();
    chooseBestNetwork();
    if(observer != NULL){observer->atStep();}
     // std::cout<<"   avg="<<calculateAverageFitness()<<std::endl;
  }

  if(network != NULL)
  {
    NetworkPopulationElement* bestNetwork = chooseBestNetwork();
    copyNetwork(bestNetwork->getNetwork(), network);
  }

  if(observer != NULL){observer->atFinish();}
}

void GeneticAlgorithmController::mutation()
{
  if(mDefaultCall == 1)
  {
    auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
    logger->info("mutation");
    spdlog::drop_all();
  }

  int numberOfMutations = mPopulation.size()*mMutationRatio;
  for(int i=0; i<numberOfMutations; ++i)
  {
    NetworkPopulationElement* networkElement = chooseForMutation();
    Network* network = networkElement->getNetwork();
    Network* mutatedNetwork = new Network;
    copyNetwork(network, mutatedNetwork);
    mutatedNetwork->setId(abs(rand())%1000000);

    if(mDefaultCall == 1)
    {
      auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
      logger->info("parent={} child={}", network->getId(), mutatedNetwork->getId());
      spdlog::drop_all();
    }

    NetworkModifier networkModifier(&mGeneticParameters);
    networkModifier.modifyNetwork(mutatedNetwork);

    loadNodesToEquations(mutatedNetwork);

    NetworkPopulationElement* mutatedElement = new NetworkPopulationElement(mutatedNetwork, mTargetVectorField, mFitnessFunction);
    mutatedElement->setRank(networkElement->getRank());
    mutatedElement->setGeneration(mGeneration);
    mPopulation.push_back(mutatedElement);
  }
}

void GeneticAlgorithmController::crossover()
{
  if(mDefaultCall == 1)
  {
    auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
    logger->info("crossover");
    spdlog::drop_all();
  }

  updateFitnessRanks();
  int numberOfCrossovers = mPopulation.size()*mCrossoverRatio;
  for(int i=0; i<numberOfCrossovers; ++i)
  {
    NetworkPopulationElement* networkElement1 = chooseForCrossover();
    NetworkPopulationElement* networkElement2 = chooseForCrossover();
    Network* parentNetwork1 = networkElement1->getNetwork();
    Network* parentNetwork2 = networkElement2->getNetwork();
    Network* childNetwork = new Network;
    createMixedNetwork(parentNetwork1, parentNetwork2, childNetwork);

    childNetwork->setId(abs(rand())%1000000);

    if(mDefaultCall == 1)
    {
      auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
      logger->info("parent={} parent={} child={}", parentNetwork1->getId(), parentNetwork2->getId(), childNetwork->getId());
      spdlog::drop_all();
    }

    NetworkPopulationElement* childElement = new NetworkPopulationElement(childNetwork, mTargetVectorField, mFitnessFunction);
    childElement->setGeneration(mGeneration);
    childElement->setRank((networkElement1->getRank()+networkElement2->getRank())/2);
    mPopulation.push_back(childElement);
  }
}

void GeneticAlgorithmController::death()
{
  updateFitnessRanks();
  int numberOfDeaths = mPopulation.size()-mInitialPopulationSize;
  for(int i=0; i<numberOfDeaths; ++i)
  {
    NetworkPopulationElement* networkElement = chooseForDeath();
    mPopulation.erase(std::remove(mPopulation.begin(), mPopulation.end(), networkElement), mPopulation.end());
    delete networkElement;
  }
}

void GeneticAlgorithmController::createInitialPopulation()
{
  for(int i=0; i<mInitialPopulationSize; ++i)
  {
    Network* newNetwork = new Network;
    newNetwork->setId(abs(rand())%1000000);
    if((i == 0) && (mNetwork != NULL))
    {
      copyNetwork(mNetwork, newNetwork);
    }
    else
    {
      mCreateInitialNetwork(newNetwork, i);
    }
    NetworkPopulationElement* populationElement = new NetworkPopulationElement(newNetwork, mTargetVectorField, mFitnessFunction);
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
  double sumRank = 0;
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    sumRank += (double)1/(*itNet)->getRank();
  }

  double random = (static_cast<double>(rand())/RAND_MAX)*sumRank;
  double counter = 0;
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    counter += (double)1/(*itNet)->getRank();
    if(counter >= random)
    {
      return (*itNet);
    }
  }

  int randomIndex = rand()%static_cast<int>(mPopulation.size());
  return mPopulation[randomIndex];
}


NetworkPopulationElement* GeneticAlgorithmController::chooseForDeath()
{
  // for(int i=2; i>=0; --i)
  // {
  //   for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  //   {
  //     int generation = (*itNet)->getGeneration();
  //     if(generation <= mGeneration-i)
  //     {
  // 	if((double)(*itNet)->getRank() < mPopulation.size()*mElitRatio)
  // 	{
  // 	  return (*itNet);
  // 	}
  //     }
  //   }
  // }
  NetworkPopulationElement* networkElement = NULL;
  while(networkElement == NULL)
  {
    networkElement = chooseForDeath_helper();
    if((double)networkElement->getRank() > mInitialPopulationSize*mElitRatio)
    {
      return networkElement;
    }
    else
    {
      networkElement = NULL;
    }
  }

  int randomIndex = rand()%static_cast<int>(mPopulation.size());
  return mPopulation[randomIndex];
}

//Helpers

NetworkPopulationElement* GeneticAlgorithmController::chooseForDeath_helper()
{
  double sumRank = 0;
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    sumRank += (*itNet)->getRank();
  }

  double random = (static_cast<double>(rand())/RAND_MAX)*sumRank;
  double counter = 0;
  for(std::vector<NetworkPopulationElement*>::iterator itNet=mPopulation.begin(); itNet != mPopulation.end(); ++itNet)
  {
    counter += (*itNet)->getRank();
    if(counter >= random)
    {
      return (*itNet);
    }
  }
  
  int randomIndex = rand()%static_cast<int>(mPopulation.size());
  return mPopulation[randomIndex];  
}

void GeneticAlgorithmController::createMixedNetwork(Network* parentNetwork1, Network* parentNetwork2, Network* childNetwork)
{
  std::vector<Node*> nodes = parentNetwork1->getNodes();
  double* tmpBuffer = new double[bufferSize];

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* parentNode = (*itNode);
    childNetwork->addNodeById(parentNode->getId());

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

    std::string strDynamicalEquation = parentNetwork->getNodeDynamicalEquationString(parentNode->getId());
    childNetwork->setDynamicalEquationString(childNode->getId(), strDynamicalEquation);
  }
  free(rand_state);
  free(rand_statebufs);
}

NetworkPopulationElement* GeneticAlgorithmController::chooseBestNetwork()
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
  // std::cout<<"   best="<<bestPopulationElement->getFitness()<<std::endl;
  return bestPopulationElement;
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

void GeneticAlgorithmController::updateFitnessRanks()
{
  std::vector<double> fitnessVector;
  for(std::vector<NetworkPopulationElement*>::iterator itPop=mPopulation.begin(); itPop != mPopulation.end(); ++itPop)
  {
    fitnessVector.push_back((*itPop)->getFitness());
  }

  quickSortTwoVectors(fitnessVector, 0, fitnessVector.size()-1);
  for(unsigned i=0; i<fitnessVector.size(); ++i)
  {
    mPopulation[i]->setRank(fitnessVector.size()-i);
  }
}

void GeneticAlgorithmController::quickSortTwoVectors(std::vector<double> &fitnessVector, int left, int right)
{
  int i = left, j = right;
  double tmpFitness;
  NetworkPopulationElement* tmpNetwork;
  double pivot = fitnessVector[(left + right) / 2];
  while (i <= j) 
  {
    while (fitnessVector[i] < pivot)
      i++;

    while (fitnessVector[j] > pivot)
      j--;

    if (i <= j) 
    {
      tmpFitness = fitnessVector[i];
      fitnessVector[i] = fitnessVector[j];
      fitnessVector[j] = tmpFitness;

      tmpNetwork = mPopulation[i];
      mPopulation[i] = mPopulation[j];
      mPopulation[j] = tmpNetwork;
      
      i++;
      j--; 
    }
  };

  if (left < j)
  {   
    quickSortTwoVectors(fitnessVector, left, j);
  }

  if (i < right)
  {
    quickSortTwoVectors(fitnessVector, i, right);
  }
}

std::vector<NetworkPopulationElement*> GeneticAlgorithmController::getPopulation()
{
  return mPopulation;
}

int GeneticAlgorithmController::getGeneration()
{
  return mGeneration;
}

void GeneticAlgorithmController::printPopulation()
{
  for(std::vector<NetworkPopulationElement*>::iterator itPop=mPopulation.begin(); itPop != mPopulation.end(); ++itPop)
  {
    std::cout<<"  element fitness="<<(*itPop)->getFitness()<<std::endl;
    Network *network = (*itPop)->getNetwork();
    network->print();
  }
}
