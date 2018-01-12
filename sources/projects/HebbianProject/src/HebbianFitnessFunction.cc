#include "HebbianFitnessFunction.hh"
#include "NetworkInitialConditionGenerators.hh"
#include "GenerateBarabasiNetwork.hh"
#include "NetworkDynamicsGenerators.hh"
#include "Node.hh"
#include "MultilayerNetwork.hh"
#include "NetworkModifier.hh"
#include "HebbianObserver.hh"
#include <iostream>
#include <random>

double hebbianFitnessFunction(NetworkPopulationElement* networkPopulationElement)
{
  int transientTime = 10;
  int runTime = 10;

  std::cout<<"Hebbian fitness"<<std::endl;

  Network* network = networkPopulationElement->getNetwork();
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  generateMultilayerNetworkForHebbianFitness(multilayerNetwork, network);

  IObserver *observer = new HebbianObserver(multilayerNetwork);
  multilayerNetwork->iterate(transientTime);
  multilayerNetwork->iterate(runTime, observer);

  delete multilayerNetwork;

  return 1;
}

void generateMultilayerNetworkForHebbianFitness(MultilayerNetwork* multilayerNetwork, Network* network)
{
  multilayerNetwork->addLayer(1);
  multilayerNetwork->addLayer(2);
  std::vector<Layer*> layers = multilayerNetwork->getLayers();

  layers[0]->addNetwork(1);
  std::vector<Network*> networks = layers[0]->getNetworks();
  Network* higherNetwork = networks[0];
  NetworkModifier networkModifier;
  networkModifier.copyNetwork(network, higherNetwork);
  randomNetworkInitialConditions(higherNetwork);

  int nodeIdCounter = 0;

  std::vector<Node*> nodes = higherNetwork->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    layers[1]->addNetwork((*itNode)->getId()+1);
    if((*itNode)->getId() > nodeIdCounter)
    {
      nodeIdCounter = (*itNode)->getId();
    }
  }

  std::vector<Network*> lowerNetworks = layers[1]->getNetworks();
  for(std::vector<Network*>::iterator itNet=lowerNetworks.begin(); itNet != lowerNetworks.end(); ++itNet)
  {
    Network* lowerNetwork = (*itNet);
    generateBarabasiNetwork(lowerNetwork, 10, nodeIdCounter);
    linearNetworkDynamicsGenerator(lowerNetwork);
    randomNetworkInitialConditions(lowerNetwork);
  }
  
  //assign networks to nodes
  std::vector<Network*> networksUp = layers[0]->getNetworks();
  std::vector<Network*> networksDown = layers[1]->getNetworks();
  int nodeCounter = 0;
  for(std::vector<Network*>::iterator itNet=networksUp.begin(); itNet != networksUp.end(); ++itNet)
  {
    std::vector<Node*> nodesInNetwork = (*itNet)->getNodes();
    for(std::vector<Node*>::iterator itNode=nodesInNetwork.begin(); itNode != nodesInNetwork.end(); ++itNode)
    {
      (*itNode)->setNetworkAssigned(networksDown[nodeCounter]);
      ++nodeCounter;
    }
  }

  multilayerNetwork->loadNodesToAllEquations();
}
