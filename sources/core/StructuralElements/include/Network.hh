#ifndef NETWORK_HH
#define NETWORK_HH

#include <vector>
#include "DynamicalEquation.hh"

class Node;
class MultilayerNetwork;

class Network
{
public:
  Network();
  Network(int);
  ~Network();

  //------------------------------------
  //-----Basic--------------------------
  //------------------------------------  

  int getId(void) const;
  void setId(int id);
  /**
     Within the network, each node has a local ID 
     that is the index of the node in the mNodes vector.
   */
  int getLocalId(int id) const;

  int getTime();
  void setTime(int time);

  Node* addNode();
  void addNodeById(int nodeId);

  void addEdge(int sourceId, int targetId);
  void removeEdge(int sourceId, int targetId);
  void removeAllEdges();

  double getEdgeWeight(int sourceId, int targetId);
  void setEdgeWeight(int sourceId, int targetId, double weight);

  DynamicalEquation* getNodeDynamicalEquation(int nodeId) const;
  void setDynamicalEquation(int nodeId, DynamicalEquation* nodeEquation);

  std::string getNodeDynamicalEquationString(int nodeId) const;
  void setDynamicalEquationString(int nodeId, std::string strEquation);

  Node* getNodeAssigned(void) const;
  void setNodeAssigned(Node *node);

  Node* getNodeById(int nodeId);
  std::vector<Node*> getNodes(void) const;
  std::vector<Node*> getNodeNeighbors(int nodeId) const;

  void setMultilayerNetwork(MultilayerNetwork *multilayerNetwork);

  /**
     Return the actual state of the network i.e. the actual 
     value of all the nodes in the network.
     @return vector of IDValuePair elements  where the first value is the ID of the node 
     and the second value is the actual value of that node
   */
  std::vector<IdValuePair> getState(void) const;
  void setState(std::vector<IdValuePair> state);

  //------------------------------------
  //-----Utility------------------------
  //------------------------------------

  int getNodeDegree(int nodeId);
  void loadNodesToEquations();
  void print();

  friend bool operator==(const Network& network1, const Network& network2);
  friend std::ostream& operator<<(std::ostream &os, const Network &network);

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
  MultilayerNetwork *mMultilayerNetwork;

  int mTime;
};

#endif
