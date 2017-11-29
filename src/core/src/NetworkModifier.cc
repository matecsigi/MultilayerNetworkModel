#include "NetworkModifier.hh"
#include <iostream>

NetworkModifier::NetworkModifier(Network* network)
{
  mNetwork = network;
}

NetworkModifier::~NetworkModifier()
{
}

void NetworkModifier::fitToVectorField(VectorField* targetVectorField)
{
  Network* networkModified = new Network;
  modifyNetwork(mNetwork, networkModified);
}

void NetworkModifier::modifyNetwork(Network* oldNetwork, Network* newNetwork)
{
  copyNetwork(oldNetwork, newNetwork);
  Node* nodeToChange = chooseNode(newNetwork);
  ModificationType type = chooseType();
  
  switch(type)
  {
  case ADD_EDGE:
    addEdge(nodeToChange);
    break;
  case REMOVE_EDGE:
    removeEdge(nodeToChange);
    break;
  case ADD_TO_OUTER_BLOCK:
    addToOuterBlock(nodeToChange);
    break;
  case REMOVE_FROM_OUTER_BLOCK:
    removeFromOuterBlock(nodeToChange);
    break;
  case CHANGE_CONSTANT:
    changeConstant(nodeToChange);
    break;
  case CHANGE_PLUS_TO_MULTIPLY:
    changePlusToMultiply(nodeToChange);
    break;
  case CHANGE_MULTIPLY_TO_PLUS:
    changeMultiplyToPlus(nodeToChange);
    break;
  default:
    std::cout<<"Default"<<std::endl;
  }
}

void NetworkModifier::copyNetwork(Network* oldNetwork, Network* newNetwork)
{
  std::vector<Node*> nodes = oldNetwork->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* oldNode = (*itNode);
    newNetwork->addNode(oldNode->getId());
    std::string strEquation = oldNetwork->getNodeDynamicalEquationString(oldNode->getId());
    newNetwork->setDynamicalEquation(oldNode->getId(), strEquation);
  }

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* oldNode = (*itNode);
    std::vector<Node*> neighbors = oldNetwork->getNodeNeighbors(oldNode->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      Node* oldNeighbor = (*itNei);
      int localId1 = newNetwork->getLocalId(oldNode->getId());
      int localId2 = newNetwork->getLocalId(oldNeighbor->getId());
      newNetwork->addEdge(localId1, localId2);
    }
  } 
}

Node* NetworkModifier::chooseNode(Network* network)
{
  std::vector<Node*> nodes = mNetwork->getNodes();
  int randomIndex = rand()%static_cast<int>(nodes.size());
  return nodes[randomIndex];
}

ModificationType NetworkModifier::chooseType()
{
  std::vector<ModificationType> typeVector;
  typeVector.push_back(ADD_EDGE);
  typeVector.push_back(REMOVE_EDGE);
  typeVector.push_back(ADD_TO_OUTER_BLOCK);
  typeVector.push_back(REMOVE_FROM_OUTER_BLOCK);
  typeVector.push_back(CHANGE_CONSTANT);
  typeVector.push_back(CHANGE_PLUS_TO_MULTIPLY);
  typeVector.push_back(CHANGE_MULTIPLY_TO_PLUS);
  
  int randomIndex = rand()%static_cast<int>(typeVector.size());

  return typeVector[randomIndex];
}

//-----------------------------------
//--------Modification functions-----
//-----------------------------------

void NetworkModifier::addEdge(Node* nodeToChange)
{

}

void NetworkModifier::removeEdge(Node* nodeToChange)
{

}

void NetworkModifier::addToOuterBlock(Node* nodeToChange)
{

}

void NetworkModifier::removeFromOuterBlock(Node* nodeToChange)
{

}

void NetworkModifier::changeConstant(Node* nodeToChange)
{

}

void NetworkModifier::changePlusToMultiply(Node* nodeToChange)
{

}

void NetworkModifier::changeMultiplyToPlus(Node* nodeToChange)
{

}
