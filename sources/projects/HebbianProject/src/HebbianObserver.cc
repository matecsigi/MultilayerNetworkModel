#include "HebbianObserver.hh"
#include "NetworkModifier.hh"
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
  std::cout<<"--atStart--"<<std::endl;
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  std::vector<Network*> lowerNetworks = layers[1]->getNetworks();
  for(std::vector<Network*>::iterator itNet=lowerNetworks.begin(); itNet != lowerNetworks.end(); ++itNet)
  {
    mLowerNetworks.push_back((*itNet));
    Network* newHebbianNetwork = new Network;
    NetworkModifier networkModifier;
    networkModifier.copyNetwork((*itNet), newHebbianNetwork);
    mHebbianNetworks.push_back(newHebbianNetwork);
  }
}

void HebbianObserver::atStep()
{
  std::cout<<"--atStep--"<<std::endl;
  for(unsigned i=0; i<mLowerNetworks.size(); ++i)
  {
    evaluateHebbianLearning(mLowerNetworks[i], mHebbianNetworks[i]);
  }
}

void HebbianObserver::atFinish()
{
  std::cout<<"--atFinish--"<<std::endl;
}

void HebbianObserver::evaluateHebbianLearning(Network* referenceNetwork, Network* network)
{
  double alpha = 0.1;

  std::cout<<"evaluateHebbianLearning"<<std::endl;
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
