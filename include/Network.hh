#ifndef NETWORK_HH
#define NETWORK_HH

#include "Node.hh"
#include <vector>

class Network
{
public:
  Network();
  Network(int);
  ~Network();

  void addNode(int nodeId);
  void addEdge(int nodeId1, int nodeId2);

  //creating a graph
  void generateConnections(void);

  int getId(void) const;
  int getLocalId(int id);
  std::vector<Node*> getNodes(void);
  std::vector<Node*> getNodeNeighbors(int nodeId);

private:
  int mNetworkId;
  Node* mNodeAssigned;
  std::vector<Node*> mNodes;
  std::vector<std::vector<Node*> > mNodeConnections;
};

#endif
