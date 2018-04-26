#ifndef NETWORKCLASSUTILITY_HH
#define NETWORKCLASSUTILITY_HH

class Network;

void loadNodesToEquations(Network *network);

int getNodeDegree(Network* network, int nodeId);

bool networksEqual(Network& network1, Network& network2);

#endif
