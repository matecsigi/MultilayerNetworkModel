#include "Node.hh"
#include "Network.hh"
#include "Trace.hh"
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <map>

Network::Network(void)
{
  mNodeAssigned = NULL;
  mMultilayerNetwork = NULL;
  mTime = 0;
}

Network::Network(int id)
{
  mNetworkId = id;
  mNodeAssigned = NULL;
  mMultilayerNetwork = NULL;
  mTime = 0;
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

int Network::getId(void) const
{
  return mNetworkId;
}

void Network::setId(int id)
{
  mNetworkId = id;
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

int Network::getTime()
{
  if(mMultilayerNetwork != NULL)
  {
    return mMultilayerNetwork->getTime();
  }
  return mTime;
}

void Network::setTime(int time)
{
  mTime = time;
}

Node* Network::addNode()
{
  int maxNodeId = 0;
  if(mMultilayerNetwork != NULL)
  {
    maxNodeId = getMaxNodeId(mMultilayerNetwork);
  }
  else
  {
    for(std::vector<Node*>::iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
    {
      if((*itNode)->getId() > maxNodeId)
      {
	maxNodeId = (*itNode)->getId();
      }
    }
  }
  Node* newNode = new Node(maxNodeId+1);
  newNode->setMultilayerNetwork(mMultilayerNetwork);
  mNodes.push_back(newNode);
  newNode->addToNetwork(this);
  std::vector<Node*> newConnectionVector;
  mNodeConnections.push_back(newConnectionVector);
  mDynamicalEquations.push_back(new DynamicalEquation(this));
  return newNode;
}

void Network::addNodeById(int nodeId)
{
  //create new node only if id doesn't exist
  Node* newNode = new Node(nodeId);
  newNode->setMultilayerNetwork(mMultilayerNetwork);
  mNodes.push_back(newNode);
  newNode->addToNetwork(this);
  std::vector<Node*> newConnectionVector;
  mNodeConnections.push_back(newConnectionVector);
  mDynamicalEquations.push_back(new DynamicalEquation(this));
}

//TODO: decide if edges should be directed or not
void Network::addEdge(int sourceId, int targetId)
{
  int localSourceId = getLocalId(sourceId);
  int localTargetId = getLocalId(targetId);
  if(std::find(mNodeConnections[localTargetId].begin(), mNodeConnections[localTargetId].end(), mNodes[localSourceId]) == mNodeConnections[localTargetId].end()) 
  {
    mNodeConnections[localTargetId].push_back(mNodes[localSourceId]);
  }
}

void Network::removeEdge(int sourceId, int targetId)
{
  int localSourceId = getLocalId(sourceId);
  int localTargetId = getLocalId(targetId);
  if(std::find(mNodeConnections[localTargetId].begin(), mNodeConnections[localTargetId].end(), mNodes[localSourceId]) != mNodeConnections[localTargetId].end()) 
  {
    mNodeConnections[localTargetId].erase(std::remove(mNodeConnections[localTargetId].begin(), mNodeConnections[localTargetId].end(), mNodes[localSourceId]), mNodeConnections[localTargetId].end());
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
      removeEdge(neighbor->getId(), node->getId());
    }
  }
}

double Network::getEdgeWeight(int sourceId, int targetId)
{
  DynamicalEquation* dynamicalEquation = getNodeDynamicalEquation(targetId);
  double weight = dynamicalEquation->getEdgeWeight(sourceId);
  return weight;
}

void Network::setEdgeWeight(int sourceId, int targetId, double weight)
{
  DynamicalEquation* dynamicalEquation = getNodeDynamicalEquation(targetId);
  dynamicalEquation->setEdgeWeight(sourceId, weight);
}

Node* Network::getNodeAssigned(void) const
{
  return mNodeAssigned;
}

void Network::setNodeAssigned(Node* node)
{
  mNodeAssigned = node;
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

void Network::setDynamicalEquation(int nodeId, DynamicalEquation* nodeEquation)
{
  int localId = getLocalId(nodeId);
  mDynamicalEquations[localId]->loadEquation(nodeEquation);
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

void Network::setDynamicalEquationString(int nodeId, std::string strEquation)
{
  int localId = getLocalId(nodeId);
  mDynamicalEquations[localId]->loadEquationString(strEquation);
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

std::vector<IdValuePair> Network::getState(void) const
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

void Network::setState(std::vector<IdValuePair> state)
{
  for(std::vector<Node*>::const_iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    int nodeId = (*itNode)->getId();
    double value = getValueForId(state, nodeId);
    (*itNode)->setCurrentState(value);
  }
}

void Network::setMultilayerNetwork(MultilayerNetwork *multilayerNetwork)
{
  mMultilayerNetwork = multilayerNetwork;
  for(std::vector<Node*>::iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    (*itNode)->setMultilayerNetwork(multilayerNetwork);
  }
}

void Network::print()
{
  traceDebug("Network "+std::to_string(mNetworkId)+"\n");
  for(std::vector<Node*>::iterator itNode=mNodes.begin(); itNode != mNodes.end(); ++itNode)
  {
    traceDebug("      ");
    (*itNode)->print();
  }
}
