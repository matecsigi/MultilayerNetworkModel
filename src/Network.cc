#include "Network.hh"
#include <iostream>

using namespace std;

Network::Network(void)
{
  cout<<"Creating network"<<endl;
}

Network::Network(int id)
{
  mNetworkId = id;
  cout<<"Creating network with id="<<mNetworkId<<endl;
}

Network::~Network(void)
{
  for(std::vector<Node*>::iterator it = mNodesInNetwork.begin(); it != mNodesInNetwork.end(); ++it)
  {
    delete (*it);
  }

}

void Network::addNode(int nodeId)
{
  mNodesInNetwork.push_back(new Node(nodeId));
}

std::vector<Node*> Network::getNodes(void)
{
  return mNodesInNetwork;
}

int Network::getId(void) const
{
  return mNetworkId;
}
