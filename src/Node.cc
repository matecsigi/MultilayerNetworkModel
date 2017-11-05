#include "Node.hh"
#include <iostream>

using namespace std;

Node::Node(void)
{
}

Node::Node(int id)
{
  mNodeId = id;
}

Node::~Node(void)
{
}

void Node::step(void)
{
  cout<<"Stepping node"<<endl;
}

void Node::addToNetwork(Network* networkPtr)
{
  mNetworks.push_back(networkPtr);
}

void Node::assignToNetwork(Network* network)
{
  mNetworkAssigned = network;
  network->assignToNode(this);

}

int Node::getId(void) const
{
  return mNodeId;
}

std::vector<Network*> Node::getNetworks(void) const
{
  return mNetworks;
}
