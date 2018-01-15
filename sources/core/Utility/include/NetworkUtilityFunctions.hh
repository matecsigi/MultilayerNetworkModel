#ifndef NETWORKUTILITYFUNCTIONS_HH
#define NETWORKUTILITYFUNCTIONS_HH

#include "Node.hh"
#include "Network.hh"
#include "VectorField.hh"

void copyNetwork(Network* oldNetwork, Network* newNetwork);
std::vector<IdValuePair> calculateLowerNetworkDirection(Node *node);
std::vector<IdValuePair> calculateHigherNetworksDirection(Node *node);

#endif
