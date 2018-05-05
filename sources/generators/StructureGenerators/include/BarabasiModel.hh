#ifndef BARABASIMODEL_HH
#define BARABASIMODEL_HH

#include <algorithm>
#include <vector>

class Network;
class Node;

void generateNetwork(Network* network, int numberOfNodes, int m=3);
Node* chooseNodePreferential(Node* newNode, Network* network);

#endif
