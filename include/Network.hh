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

  std::vector<Node*> getNodes(void);

private:
  int mNetworkId;
  std::vector<Node*> mNodesInNetwork;
};

#endif
