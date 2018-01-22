#include "HebbianObserver.hh"
#include "VectorFieldSchemes.hh"
#include "NetworkUtilityFunctions.hh"
#include <iostream>

HebbianObserver::~HebbianObserver()
{
  for(std::vector<Network*>::iterator itNet=mHebbianNetworks.begin(); itNet != mHebbianNetworks.end(); ++itNet)
  {
    delete (*itNet);
  }
}

void HebbianObserver::atStart()
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  std::vector<Network*> lowerNetworks = layers[1]->getNetworks();
  for(std::vector<Network*>::iterator itNet=lowerNetworks.begin(); itNet != lowerNetworks.end(); ++itNet)
  {
    mLowerNetworks.push_back((*itNet));
    Network* newHebbianNetwork = new Network;
    copyNetwork((*itNet), newHebbianNetwork);
    mHebbianNetworks.push_back(newHebbianNetwork);
  }
}

void HebbianObserver::atStep()
{
  for(unsigned i=0; i<mLowerNetworks.size(); ++i)
  {
    evaluateHebbianLearning(mLowerNetworks[i], mHebbianNetworks[i]);
  }
}

void HebbianObserver::atFinish()
{
  for(unsigned i=0; i<mLowerNetworks.size(); ++i)
  {
    Network* realNetwork = mLowerNetworks[i];
    Network* hebbianNetwork = mHebbianNetworks[i];

    VectorField* realVectorField = new VectorField();
    std::vector<IdValuePair> currentState = realNetwork->getCurrentState();
    gridAroundPointScheme2(realVectorField, realNetwork, currentState);

    VectorField* hebbianVectorField = new VectorField();
    gridAroundPointScheme2(hebbianVectorField, hebbianNetwork, currentState);    

    double distance = realVectorField->getDistanceFrom(hebbianVectorField);
    mDistances.push_back(distance);

    delete realVectorField;
    delete hebbianVectorField;
  }

  double sumDistances = 0;
  for(std::vector<double>::iterator itDis=mDistances.begin(); itDis != mDistances.end(); ++itDis)
  {
    sumDistances += *itDis;
  }
}

double HebbianObserver::getResult()
{
  double sumDistances = 0;
  for(std::vector<double>::iterator itDis=mDistances.begin(); itDis != mDistances.end(); ++itDis)
  {
    sumDistances += *itDis;
  }
  sumDistances = sumDistances/(double)(mDistances.size());

  return sumDistances;  
}

void HebbianObserver::evaluateHebbianLearning(Network* referenceNetwork, Network* network)
{
  double alpha = 0.1;
  std::vector<Node*> nodes = referenceNetwork->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* targetNode = *itNode;
    double targetCurrentState = targetNode->getCurrentState();
    std::vector<Node*> neighbors = referenceNetwork->getNodeNeighbors(targetNode->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      Node* sourceNode = *itNei;
      double sourcePreviousState = sourceNode->getPreviousState();
      double edgeWeight = network->getEdgeWeight(sourceNode->getId(), targetNode->getId());
      double deltaEdgeWeight = alpha*(sourcePreviousState*targetCurrentState-targetCurrentState*targetCurrentState*edgeWeight);
      network->setEdgeWeight(sourceNode->getId(), targetNode->getId(), edgeWeight+deltaEdgeWeight);
    }
  }
}
