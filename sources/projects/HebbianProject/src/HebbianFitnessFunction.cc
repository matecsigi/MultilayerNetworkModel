#include "HebbianFitnessFunction.hh"
#include "NetworkInitialConditionGenerators.hh"
#include <iostream>

double hebbianFitnessFunction(NetworkPopulationElement* networkPopulationElement)
{
  std::cout<<"Hebbian fitness"<<std::endl;

  Network* network = networkPopulationElement->getNetwork();

  //generate starting state for network
  randomNetworkInitialConditions(network);
  std::cout<<*network<<std::endl;
  //generate lower network with barabasi
  //generate lower network dynamics
  //generate lower starting conditions
  //assign lower network to higher node

  //ODE step higher
  //ODE step lower
  //legyen upward/downward influence?
  //lower vector reconfig, edge weight only

  //calculate hebbian for lower -> hebbianNetwork
  
  //distance between lower and hebbianNetwork

  return 1;
}
