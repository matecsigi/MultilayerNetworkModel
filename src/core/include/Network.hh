#ifndef NETWORK_HH
#define NETWORK_HH

#include <vector>
#include "DynamicalEquation.hh"

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
  
  void setDynamicalEquation(int nodeId, std::string strEquation);

  //creating a graph
  void generateConnections(void);

  int getId(void) const;
  int getLocalId(int id) const;
  Node* getNodeAssigned(void) const;
  std::vector<Node*> getNodes(void) const;
  std::vector<Node*> getNodeNeighbors(int nodeId) const;
  DynamicalEquation* getNodeDynamicalEquation(int nodeId) const;
  std::string getNodeDynamicalEquationString(int nodeId) const;
  std::map<int, double> getCurrentState(void) const;
  std::map<int, double> getDirectionAtState(std::map<int, double> basePointCoordinates) const;

  friend bool operator==(const Network& network1, const Network& network2);

private:
  int mNetworkId;
  Node* mNodeAssigned;
  std::vector<Node*> mNodes;
  std::vector<std::vector<Node*> > mNodeConnections;
  std::vector<DynamicalEquation*> mDynamicalEquations;
};

#endif
