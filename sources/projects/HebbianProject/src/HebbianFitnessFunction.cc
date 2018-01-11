#include "HebbianFitnessFunction.hh"
#include "NetworkInitialConditionGenerators.hh"
#include "GenerateBarabasiNetwork.hh"
#include "NetworkDynamicsGenerators.hh"
#include "Node.hh"
#include <iostream>
#include <random>

double hebbianFitnessFunction(NetworkPopulationElement* networkPopulationElement)
{
  std::cout<<"Hebbian fitness"<<std::endl;

  Network* network = networkPopulationElement->getNetwork();

  //generate starting state for network
  randomNetworkInitialConditions(network);

  //generate lower network with barabasi
  //generate lower network dynamics
  Network* lowerNetwork = new Network;
  generateBarabasiNetwork(lowerNetwork, 10);
  linearNetworkDynamicsGenerator(lowerNetwork);

  //generate lower starting conditions
  randomNetworkInitialConditions(lowerNetwork);

  //assign lower network to higher node
  std::vector<Node*> higherNodes = network->getNodes();
  int randomIndex = rand()%higherNodes.size();
  Node* higherNode = higherNodes[randomIndex];
  higherNode->setNetworkAssigned(lowerNetwork);

  //ODE step higher
  //ODE step lower
  //legyen upward/downward influence?
  //lower vector reconfig, edge weight only

  //calculate hebbian for lower -> hebbianNetwork
  
  //distance between lower and hebbianNetwork

  delete network;
  delete lowerNetwork;

  return 1;
}
