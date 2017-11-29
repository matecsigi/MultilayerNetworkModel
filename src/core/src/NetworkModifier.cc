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
  copyNetwork(mNetwork, networkModified);
  modifyNetwork(networkModified);
}

void NetworkModifier::modifyNetwork(Network* network)
{
  Node* nodeToChange = chooseNode(network);
  ModificationType type = chooseType();

  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(nodeToChange->getId());
  
  switch(type)
  {
  case ADD_EDGE:
    addEdge(nodeEquation);
    break;
  case REMOVE_EDGE:
    removeEdge(nodeEquation);
    break;
  case ADD_TO_OUTER_BLOCK:
    addToOuterBlock(nodeEquation);
    break;
  case REMOVE_FROM_OUTER_BLOCK:
    removeFromOuterBlock(nodeEquation);
    break;
  case CHANGE_CONSTANT:
    changeConstant(nodeEquation);
    break;
  case CHANGE_PLUS_TO_MULTIPLY:
    changePlusToMultiply(nodeEquation);
    break;
  case CHANGE_MULTIPLY_TO_PLUS:
    changeMultiplyToPlus(nodeEquation);
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

void NetworkModifier::addEdge(DynamicalEquation* nodeEquation)
{

}

void NetworkModifier::removeEdge(DynamicalEquation* nodeEquation)
{

}

void NetworkModifier::addToOuterBlock(DynamicalEquation* nodeEquation)
{

}

void NetworkModifier::removeFromOuterBlock(DynamicalEquation* nodeEquation)
{

}

void NetworkModifier::changeConstant(DynamicalEquation* nodeEquation)
{
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();
  int nrOfConstants = numberOfType(baseCalcNode, CONSTANT);
  std::cout<<nrOfConstants<<std::endl;
  if(nrOfConstants == 0)
  {
    return;
  }
  int randomConstantElement = rand()%static_cast<int>(nrOfConstants);
  changeSpecifiedConstant(baseCalcNode, randomConstantElement, 0);
}

void NetworkModifier::changePlusToMultiply(DynamicalEquation* nodeEquation)
{
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();
  int nrOfAdds = numberOfType(baseCalcNode, ADD);
  if(nrOfAdds == 0)
  {
    return;
  }
  int randomPlusElement = rand()%static_cast<int>(nrOfAdds);
  baseCalcNode = changeSpecifiedPlusToMultiply(baseCalcNode, randomPlusElement);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
}

void NetworkModifier::changeMultiplyToPlus(DynamicalEquation* nodeEquation)
{
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();
  int nrOfMultiplies = numberOfType(baseCalcNode, MULTIPLY);
  if(nrOfMultiplies == 0)
  {
    return;
  }
  int randomMultiplyElement = rand()%static_cast<int>(nrOfMultiplies);
  baseCalcNode = changeSpecifiedMultiplyToPlus(baseCalcNode, randomMultiplyElement);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
}

//-----------------------------------
//--------Helper functions-----------
//-----------------------------------

int NetworkModifier::numberOfType(CalculationNode* calcNode, CalcNodeTypes type)
{
  int localCounter = 0;
  if(calcNode->getType() == type)
  {
    localCounter += 1;
  }

  if(calcNode->left != NULL)
  {
    localCounter += numberOfType(calcNode->left, type);
  }
  if(calcNode->right != NULL)
  {
    localCounter += numberOfType(calcNode->right, type);
  }

  return localCounter;
}

void NetworkModifier::changeSpecifiedConstant(CalculationNode* calcNode, int elementIndex, int elementCounter)
{
  if(calcNode->getType() == CONSTANT)
  {
    if(elementCounter == elementIndex)
    {
      double value = calcNode->getValue();
      //random number in the [-1, 1] range
      value += -1+2*((double)rand()/RAND_MAX);
      calcNode->setValue(value);
    }
    elementCounter += 1;
  }

  if(calcNode->left != NULL)
  {
    changeSpecifiedConstant(calcNode->left, elementIndex, elementCounter);
  }
  if(calcNode->right != NULL)
  {
    changeSpecifiedConstant(calcNode->right, elementIndex, elementCounter);
  }

  return;  
}

CalculationNode* NetworkModifier::changeSpecifiedPlusToMultiply(CalculationNode* baseCalcNode, int elementIndex)
{
  CalculationNode* calcNode = NULL;
  getSpecifiedElementFromType(calcNode, baseCalcNode, ADD, elementIndex, 0);
  if(calcNode == baseCalcNode)
  {
    CalculationNode* multiplyNode = new CNMultiply(calcNode->left, calcNode->right);
    calcNode->left = NULL;
    calcNode->right = NULL;
    baseCalcNode = multiplyNode;
    delete calcNode;
    calcNode = multiplyNode;
  }
  else
  {
    CalculationNode* parentCalcNode = getParent(baseCalcNode, calcNode);
    CalculationNode* multiplyNode = new CNMultiply(calcNode->left, calcNode->right);
    calcNode->left = NULL;
    calcNode->right = NULL;
    if(parentCalcNode->left == calcNode)
    {
      parentCalcNode->left = multiplyNode;
    }
    else if(parentCalcNode->right == calcNode)
    {
      parentCalcNode->right = multiplyNode;
    }
    delete calcNode;
    calcNode = multiplyNode;
  }

  return baseCalcNode;
}

CalculationNode* NetworkModifier::changeSpecifiedMultiplyToPlus(CalculationNode* baseCalcNode, int elementIndex)
{
  CalculationNode* calcNode = NULL;
  getSpecifiedElementFromType(calcNode, baseCalcNode, MULTIPLY, elementIndex, 0);
  if(calcNode == baseCalcNode)
  {
    CalculationNode* addNode = new CNAdd(calcNode->left, calcNode->right);
    calcNode->left = NULL;
    calcNode->right = NULL;
    baseCalcNode = addNode;
    delete calcNode;
    calcNode = addNode;
  }
  else
  {
    CalculationNode* parentCalcNode = getParent(baseCalcNode, calcNode);
    CalculationNode* addNode = new CNAdd(calcNode->left, calcNode->right);
    calcNode->left = NULL;
    calcNode->right = NULL;
    if(parentCalcNode->left == calcNode)
    {
      parentCalcNode->left = addNode;
    }
    else if(parentCalcNode->right == calcNode)
    {
      parentCalcNode->right = addNode;
    }
    delete calcNode;
    calcNode = addNode;
  }

  return baseCalcNode;
}

void NetworkModifier::getSpecifiedElementFromType(CalculationNode* &storeCalcNode, CalculationNode* stepCalcNode, CalcNodeTypes type, int elementIndex, int elementCounter)
{
  if(stepCalcNode->getType() == type)
  {
    if(elementCounter == elementIndex)
    {
      storeCalcNode = stepCalcNode;
    }
    elementCounter += 1;
  }

  if(stepCalcNode->left != NULL)
  {
    getSpecifiedElementFromType(storeCalcNode, stepCalcNode->left, type, elementIndex, elementCounter);
  }
  if(stepCalcNode->right != NULL)
  {
    getSpecifiedElementFromType(storeCalcNode, stepCalcNode->right, type, elementIndex, elementCounter);
  }

  return;  
}

CalculationNode* NetworkModifier::getParent(CalculationNode* calcNode, CalculationNode* childCalcNode)
{
  if((calcNode->left == childCalcNode) && (calcNode->right == childCalcNode))
  {
    return calcNode;
  }

  if(calcNode->left != NULL)
  {
    CalculationNode* parent = getParent(calcNode->left, childCalcNode);
    if(parent != NULL)
    {
      return parent;
    }
  }
  if(calcNode->right != NULL)
  {
    CalculationNode* parent = getParent(calcNode->right, childCalcNode);
    if(parent != NULL)
    {
      return parent;
    }    
  }

  return NULL;    
}
