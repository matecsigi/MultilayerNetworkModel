#ifndef NODE_HH
#define NODE_HH

#include <vector>

class Network;

class Node
{
public:
  Node();
  Node(int);
  ~Node();

  void step(); //set return type if necessary

  void addToNetwork(Network* networkPtr);

  int getId(void) const;
std::vector<Network*> getNetworks(void) const;

private:
  int mNodeId;
  std::vector<Network*> mNetworks;
  Network* mNetworkAssigned; //should be one
};
#endif
