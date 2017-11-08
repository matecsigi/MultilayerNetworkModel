#ifndef NETWORK_HH
#define NETWORK_HH

#include <vector>

class Node;

class Network
{
public:
  Network();
  Network(int);
  ~Network();

  void addNode(int nodeId);
  void addEdge(int localNodeId1, int localNodeId2);
  void assignToNode(Node *node);
  
  //creating a graph
  void generateConnections(void);

  int getId(void) const;
  int getLocalId(int id);
  Node* getNodeAssigned(void) const;
  std::vector<Node*> getNodes(void);
  std::vector<Node*> getNodeNeighbors(int nodeId);

private:
  int mNetworkId;
  Node* mNodeAssigned;
  std::vector<Node*> mNodes;
  std::vector<std::vector<Node*> > mNodeConnections;
};

#endif
