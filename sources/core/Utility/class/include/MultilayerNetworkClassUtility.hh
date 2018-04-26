#ifndef MULTILAYERNETWORKCLASSUTILITY_HH
#define MULTILAYERNETWORKCLASSUTILITY_HH

#include "SimulationParameterContainer.hh"
#include <vector>
#include <map>

class MultilayerNetwork;
class Network;
class Node;

/**
   When the end of buffers is reached i.e. they are filled 
   with the previous values of the nodes, the contents of the 
   current buffers is writen to a file, and the buffers are set 
   back to all 0 elements, except for the first two values that 
   contain the end values of the previous buffer.
*/
void shiftBuffers(MultilayerNetwork *multilayerNetwork);

double getMaxValue(MultilayerNetwork *multilayerNetwork);
double getMinValue(MultilayerNetwork *multilayerNetwork);

void updateNodesMap(MultilayerNetwork *multilayerNetwork);
void collectNodes(MultilayerNetwork *multilayerNetwork, std::map<int, Node*>& nodesMap, std::vector<int>& nodeIds);
void collectNetworks(MultilayerNetwork *multilayerNetwork, std::map<int, Network*>& networksMap, std::vector<int>& networkIds);

bool initialConditionsEqual(MultilayerNetwork& multilayerNetwork1, MultilayerNetwork& multilayerNetwork2);
bool dynamicalEquationsEqual(MultilayerNetwork& multilayerNetwork1, MultilayerNetwork& multilayerNetwork2);
bool multilayerNetworksEqual(MultilayerNetwork& multilayerNetwork1, MultilayerNetwork& multilayerNetwork2);

void calculateClusterMessageSizes(MultilayerNetwork *multilayerNetwork, SimulationParameterContainer *parameters);

#endif
