#ifndef NODE_HH
#define NODE_HH

#include "Network.hh"
#include <vector>

class Node
{
public:
  Node();
  Node(int);
  ~Node();

  void step(); //set return type if necessary

  void addToNetwork(Network* networkPtr);
  void assignToNetwork(Network* network);

  int getId(void) const;
std::vector<Network*> getNetworks(void) const;

private:
  int mNodeId;
  std::vector<Network*> mNetworks;
  Network* mNetworkAssigned; //should be one
};
#endif
