#include "Node.hh"
#include "Network.hh"
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <map>

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
void Network::addEdge(int nodeId1, int nodeId2)
{
  int localNodeId1 = getLocalId(nodeId1);
  int localNodeId2 = getLocalId(nodeId2);
  if(std::find(mNodeConnections[localNodeId1].begin(), mNodeConnections[localNodeId1].end(), mNodes[localNodeId2]) == mNodeConnections[localNodeId1].end()) 
  {
    mNodeConnections[localNodeId1].push_back(mNodes[localNodeId2]);
  }
}

void Network::removeEdge(int nodeId1, int nodeId2)
{
  int localId1 = getLocalId(nodeId1);
  int localId2 = getLocalId(nodeId2);
  if(std::find(mNodeConnections[localId1].begin(), mNodeConnections[localId1].end(), mNodes[localId2]) != mNodeConnections[localId1].end()) 
  {
    mNodeConnections[localId1].erase(std::remove(mNodeConnections[localId1].begin(), mNodeConnections[localId1].end(), mNodes[localId2]), mNodeConnections[localId1].end());
  }  
}

void Network::removeAllEdges()
{
  for(std::vector<Node*>::iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    Node* node = (*itNode);
    std::vector<Node*> neighbors = getNodeNeighbors(node->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      Node* neighbor = (*itNei);
      removeEdge(node->getId(), neighbor->getId());
    }
  }
}

void Network::assignToNode(Node* node)
{
  mNodeAssigned = node;
}

void Network::setDynamicalEquation(int nodeId, DynamicalEquation* nodeEquation)
{
  int localId = getLocalId(nodeId);
  mDynamicalEquations[localId]->loadEquation(nodeEquation);
}

void Network::setDynamicalEquationString(int nodeId, std::string strEquation)
{
  int localId = getLocalId(nodeId);
  mDynamicalEquations[localId]->loadEquationString(strEquation);
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

std::vector<IdValuePair> Network::getCurrentState(void) const
{
  std::vector<IdValuePair> currentState;
  for(std::vector<Node*>::const_iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    int nodeId = (*itNode)->getId();
    int nodeState = (*itNode)->getPreviousState();
    currentState.push_back(IdValuePair(nodeId, nodeState));
  }

  return currentState;
}

std::vector<IdValuePair> Network::getDirectionAtState(std::vector<IdValuePair> &basePointCoordinates) const
{
  std::vector<IdValuePair> directions;
  std::vector<IdValuePair> finalState;
  for(std::vector<Node*>::const_iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    DynamicalEquation* nodeEquation = getNodeDynamicalEquation((*itNode)->getId());
    (*itNode)->stepOdeAtState(nodeEquation, basePointCoordinates, finalState);
  }
  
  for(std::vector<IdValuePair>::iterator itState=basePointCoordinates.begin(); itState != basePointCoordinates.end(); ++itState)
   {
     int id = itState->mId;
     double startValue = getValueForId(basePointCoordinates, id);
     double finalValue = getValueForId(finalState, id);
     setValueForId(directions, id, finalValue-startValue);
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

Node* Network::getNodeById(int nodeId)
{
  for(std::vector<Node*>::iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    Node* currentNode = (*itNode);
    if(currentNode->getId() == nodeId)
    {
      return currentNode;
    }
  }
  return NULL;
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
