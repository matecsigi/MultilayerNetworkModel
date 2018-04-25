#ifndef NETWORKUTILITYFUNCTIONS_HH
#define NETWORKUTILITYFUNCTIONS_HH

#include "Node.hh"
#include "Network.hh"
#include "VectorField.hh"

void copyNetwork(Network* oldNetwork, Network* newNetwork);
  /**
     Return the direction in which the network is currently moving 
     in the state space i.e. the derivative in the phase space.
     Calculated by stepping the network one step and measuring the 
     changes in the axis directions.
     @param basePointCoordinates: the coordinates of the current state 
     in the phase space
     @return vector of IdValuePairelements where first value is the ID of the node and the 
     secodn value is the change along the node's axis i.e. partial 
     derivative.
   */
std::vector<IdValuePair> getIsolatedDirectionAtState(Network* network, std::vector<IdValuePair> &basePointCoordinates);
std::vector<IdValuePair> getEnvironmentalDirectionAtState(Network* network, std::vector<IdValuePair> &basePointCoordinates);
Network* createEnvironmentalMultilayerNetwork(MultilayerNetwork* multilayerNetwork, Network* network);

#endif
