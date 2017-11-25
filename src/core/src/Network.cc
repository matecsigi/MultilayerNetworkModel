#include "Network.hh"
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <map>

#include "Node.hh"
using namespace std;

Network::Network(void)
{
  mNodeAssigned = NULL;
}

Network::Network(int id)
{
  mNetworkId = id;
  mNodeAssigned = NULL;
}

Network::~Network(void)
{
  for(std::vector<Node*>::iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    delete (*itNode);
  }
  for(std::vector<DynamicalEquation*>::iterator itEq=mDynamicalEquations.begin(); itEq != mDynamicalEquations.end(); ++itEq)
  {
    delete (*itEq);
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
  mDynamicalEquations.push_back(new DynamicalEquation());
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

  return;
}

void Network::assignToNode(Node* node)
{
  mNodeAssigned = node;
}

void Network::setDynamicalEquation(int nodeId, std::string strEquation)
{
  int localId = getLocalId(nodeId);
  mDynamicalEquations[localId]->loadEquation(strEquation);
}

void Network::generateConnections(void)
{
  //creating a circular graph
  for(unsigned i=1; i<mNodes.size(); ++i)
  {
    addEdge(i-1, i);
  }
  if(mNodes.size() > 1)
  {
    addEdge(mNodes.size()-1, 0);
  }
}

int Network::getId(void) const
{
  return mNetworkId;
}

int Network::getLocalId(int id) const
{
  int counter = 0;
  for(std::vector<Node*>::const_iterator it=mNodes.begin(); it != mNodes.end(); ++it)
  {
    if((*it)->getId() == id)
    {
      return counter;
    }
    ++counter;
  }
  return -1;
}

Node* Network::getNodeAssigned(void) const
{
  return mNodeAssigned;
}

std::vector<Node*> Network::getNodes(void) const
{
  return mNodes;
}

std::vector<Node*> Network::getNodeNeighbors(int nodeId) const
{
  int localId = getLocalId(nodeId);
  if(localId != -1)
  {
    return mNodeConnections[localId];
  }
  return {};
}

DynamicalEquation* Network::getNodeDynamicalEquation(int nodeId) const
{
  int localId = getLocalId(nodeId);
  if(localId != -1)
  {
    return mDynamicalEquations[localId];
  }
  return NULL;
}

std::map<int, double> Network::getCurrentState(void) const
{
  std::map<int, double> currentState;
  for(std::vector<Node*>::const_iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    currentState[(*itNode)->getId()] = (*itNode)->getPreviousState();
  }

  return currentState;
}

std::map<int, double> Network::getDirectionAtState(std::map<int, double> basePointCoordinates) const
{
  std::map<int, double> directions;
  std::map<int, double> finalState;
  for(std::vector<Node*>::const_iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    DynamicalEquation* nodeEquation = getNodeDynamicalEquation((*itNode)->getId());
    (*itNode)->stepOdeAtState(nodeEquation, basePointCoordinates, finalState);
  }
  
  for(std::map<int, double>::iterator itState=basePointCoordinates.begin(); itState != basePointCoordinates.end(); ++itState)
   {
     int id = itState->first;
     double startValue = basePointCoordinates[id];
     double finalValue = finalState[id];
     directions[id] = finalValue-startValue;
   }

  return directions;
}

std::string Network::getNodeDynamicalEquationString(int nodeId) const
{
  int localId = getLocalId(nodeId);
  if(localId != -1)
  {
    std::string result = mDynamicalEquations[localId]->toString();
    return result;
  }
  return "";
}

bool operator==(const Network& network1, const Network& network2)
{
  std::vector<Node*> nodes1 = network1.getNodes();
  std::vector<Node*> nodes2 = network2.getNodes();
  if(nodes1.size() != nodes2.size())
  {
    return false;
  }

  std::map<int, Node*> nodeMap;
  for(std::vector<Node*>::iterator itNode=nodes1.begin(); itNode != nodes1.end(); ++itNode)
  {
    nodeMap[(*itNode)->getId()] = (*itNode);
  }

  for(std::vector<Node*>::iterator itNode=nodes2.begin(); itNode != nodes2.end(); ++itNode)
  {
    if(nodeMap.count((*itNode)->getId()))
    {
      Node* currentNode = (*itNode);
      Node* correspondingNode = nodeMap[currentNode->getId()];
      if(!(*currentNode == *correspondingNode))
      {
	return false;
      }
      
      std::vector<Node*> neighbors1 = network1.getNodeNeighbors(currentNode->getId());
      std::vector<Node*> neighbors2 = network2.getNodeNeighbors(currentNode->getId());
      if(neighbors1.size() != neighbors2.size())
      {
	return false;
      }

      std::map<int, Node*> neighborMap;
      for(std::vector<Node*>::iterator itNei=neighbors1.begin(); itNei != neighbors1.end(); ++itNei)
      {
	neighborMap[(*itNei)->getId()] = (*itNei);
      }

      for(std::vector<Node*>::iterator itNei=neighbors2.begin(); itNei != neighbors2.end(); ++itNei)
      {
	if(!(neighborMap.count((*itNei)->getId())))
	{
	  return false;
	}
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}
