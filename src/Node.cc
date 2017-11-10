#include "Node.hh"
#include <iostream>

using namespace std;

Node::Node(void)
{
  mNetworkAssigned = NULL;
  mValues = new double[2];
}

Node::Node(int id)
{
  mNodeId = id;
  mNetworkAssigned = NULL;
  mValues = new double[2];
}

Node::~Node(void)
{
  delete [] mValues;
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

void Node::setValues(double* values)
{
  mValues[0] = values[0];
  mValues[1] = values[1];
}

int Node::getId(void) const
{
  return mNodeId;
}

Network* Node::getNetworkAssigned(void) const
{
  return mNetworkAssigned;
}

std::vector<Network*> Node::getNetworks(void) const
{
  return mNetworks;
}

void Node::getValues(double* values)
{
  values[0] = mValues[0];
  values[1] = mValues[1];
}

bool operator==(const Node& node1, const Node& node2)
{
  Network* networkAssigned1 = node1.getNetworkAssigned();
  Network* networkAssigned2 = node2.getNetworkAssigned();
  
  if((networkAssigned1 == NULL) && (networkAssigned2 == NULL))
  {
    return true;
  }
  else if((networkAssigned1 != NULL) && (networkAssigned2 == NULL))
  {
    return false;
  }
  else if((networkAssigned1 == NULL) && (networkAssigned2 != NULL))
  {
    return false;
  }
  else if(networkAssigned1->getId() != networkAssigned2->getId())
  {
    return false;
  }

  return true;
}
