#include "Node.hh"
#include <iostream>

using namespace std;

Node::Node(void)
{
  cout<<"New node created"<<endl;
}

Node::Node(int id)
{
  mNodeId = id;
  cout<<"New node with id="<<mNodeId<<endl;
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

int Node::getId(void) const
{
  return mNodeId;
}

std::vector<Network*> Node::getNetworks(void) const
{
  return mNetworks;
}
