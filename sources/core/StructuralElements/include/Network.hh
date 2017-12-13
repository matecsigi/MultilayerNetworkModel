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
  void addEdge(int nodeId1, int nodeId2);
  void removeEdge(int nodeId1, int nodeId2);
  void removeAllEdges();

  void assignToNode(Node *node);
  void setDynamicalEquation(int nodeId, DynamicalEquation* nodeEquation);
  void setDynamicalEquationString(int nodeId, std::string strEquation);

  /**
     Return the unique ID of the network.
   */
  int getId(void) const;

  /**
     Within the network, each node has a local ID 
     that is the index of the node in the mNodes vector.
   */
  int getLocalId(int id) const;
  Node* getNodeAssigned(void) const;

  /**
     Return a pointer to the node that corresponds to 
     the given node ID.
     @param nodeId: the node ID
   */
  Node* getNodeById(int nodeId);
  std::vector<Node*> getNodes(void) const;

  /**
     Return the list of neighbors of a given node.
     @param nodeId: the node ID
   */
  std::vector<Node*> getNodeNeighbors(int nodeId) const;

  /**
     Return a pointer to the DynamicalEquation of the 
     given node.
     @param nodeId: the node ID
   */
  DynamicalEquation* getNodeDynamicalEquation(int nodeId) const;
  std::string getNodeDynamicalEquationString(int nodeId) const;

  //Related to vector fields

  /**
     Return the actual state of the network i.e. the actual 
     value of all the nodes in the network.
     @return vector of IDValuePair elements  where the first value is the ID of the node 
     and the second value is the actual value of that node
   */
  std::vector<IdValuePair> getCurrentState(void) const;

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
  std::vector<IdValuePair> getDirectionAtState(std::vector<IdValuePair> &basePointCoordinates) const;

  friend bool operator==(const Network& network1, const Network& network2);

private:
  int mNetworkId;
  
  /**
     Each network is assigned to a node one layer higher.
   */
  Node* mNodeAssigned;
  std::vector<Node*> mNodes;

  /**
     For each node (A) contains the list of the nodes (B) 
     for which there is an edge B->A in the network.
   */
  std::vector<std::vector<Node*> > mNodeConnections;

  /**
     For each node stores the differential equation for the 
     given node.
   */
  std::vector<DynamicalEquation*> mDynamicalEquations;
};

#endif
