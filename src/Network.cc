#include "Network.hh"
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "Node.hh"
using namespace std;

Network::Network(void)
{
}

Network::Network(int id)
{
  mNetworkId = id;
}

Network::~Network(void)
{
  for(std::vector<Node*>::iterator it = mNodes.begin(); it != mNodes.end(); ++it)
  {
    delete (*it);
  }

}

void Network::addNode(int nodeId)
{
  //create new node only if id doesn't exist
  Node* newNode = new Node(nodeId);
  mNodes.push_back(newNode);
  newNode->addToNetwork(this);
  std::vector<Node*> newConnectionVector;
  mNodeConnections.push_back(newConnectionVector);
}

//TODO: decide if edges should be directed or not
void Network::addEdge(int localNodeId1, int localNodeId2)
{
  //check if has nodes
  // int localId1 = getLocalId(nodeId1);
  // int localId2 = getLocalId(nodeId2);
  //error handling
  //-1 is network has no node with given id
  // assert(localId1 != -1);
  // assert(localId2 != -1);
  
  //check if edge exists
  if(std::find(mNodeConnections[localNodeId1].begin(), mNodeConnections[localNodeId1].end(), mNodes[localNodeId2]) == mNodeConnections[localNodeId1].end()) 
  {
    mNodeConnections[localNodeId1].push_back(mNodes[localNodeId2]);
  }

  //uncomment to have undirected edges
  // if(std::find(mNodeConnections[localId2].begin(), mNodeConnections[localId2].end(), mNodes[localId1]) == mNodeConnections[localId2].end())
  // {
  //   mNodeConnections[localId2].push_back(mNodes[localId1]);
  // }

  return;
}

void Network::assignToNode(Node* node)
{
  mNodeAssigned = node;
}

void Network::generateConnections(void)
{
  //creating a circular graph
  for(unsigned i=1; i<mNodes.size(); ++i)
  {
    addEdge(i, i-1);
  }
}

int Network::getId(void) const
{
  return mNetworkId;
}

int Network::getLocalId(int id)
{
  int counter = 0;
  for(std::vector<Node*>::iterator it=mNodes.begin(); it != mNodes.end(); ++it)
  {
    if((*it)->getId() == id)
    {
      return counter;
    }
    ++counter;
  }
  return -1;
}

std::vector<Node*> Network::getNodes(void)
{
  return mNodes;
}

std::vector<Node*> Network::getNodeNeighbors(int nodeId)
{
  int localId = getLocalId(nodeId);
  if(localId != -1)
  {
    return mNodeConnections[localId];
  }
  return {};
}
