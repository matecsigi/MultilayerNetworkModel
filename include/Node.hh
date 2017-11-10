#ifndef NODE_HH
#define NODE_HH

#include "Network.hh"
#include "IUpwardInfluence.hh"
#include <vector>

class IUpwardInfluence;

class Node
{
public:
  Node();
  Node(int);
  ~Node();

  void step(); //set return type if necessary

  void addToNetwork(Network* networkPtr);
  void assignToNetwork(Network* network);
  void setValues(double* values);
  void setUpwardInfluence();

  int getId(void) const;
  Network* getNetworkAssigned(void) const;
  std::vector<Network*> getNetworks(void) const;
  void getValues(double* values);

  friend bool operator==(const Node& node1, const Node& node2);

private:
  int mNodeId;
  Network* mNetworkAssigned;
  std::vector<Network*> mNetworks;
  double* mValues;
  IUpwardInfluence* mUpwardInfluence;
};
#endif
