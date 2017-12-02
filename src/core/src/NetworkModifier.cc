#include "NetworkModifier.hh"
#include <iostream>
#include <algorithm>

NetworkModifier::NetworkModifier()
{
}

NetworkModifier::~NetworkModifier()
{
}

void NetworkModifier::modifyNetwork(Network* network)
{
  Node* nodeToChange = chooseNode(network);
  ModificationType type = chooseType();

  switch(type)
  {
  case ADD_EDGE:
    addEdge(network, nodeToChange);
    break;
  case REMOVE_EDGE:
    removeEdge(network, nodeToChange);
    break;
  case ADD_TO_OUTER_BLOCK:
    addToOuterBlock(network, nodeToChange);
    break;
  case REMOVE_FROM_OUTER_BLOCK:
    removeFromOuterBlock(network, nodeToChange);
    break;
  case CHANGE_CONSTANT:
    changeConstant(network, nodeToChange);
    break;
  case CHANGE_PLUS_TO_MULTIPLY:
    changePlusToMultiply(network, nodeToChange);
    break;
  case CHANGE_MULTIPLY_TO_PLUS:
    changeMultiplyToPlus(network, nodeToChange);
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
    Node* newNode = newNetwork->getNodeById(oldNode->getId());
    double* tmpBuffer = new double[bufferSize];
    oldNode->getValues(tmpBuffer);
    newNode->setValues(tmpBuffer);
    delete [] tmpBuffer;

    std::string strEquation = oldNetwork->getNodeDynamicalEquationString(oldNode->getId());
    newNetwork->setDynamicalEquation(oldNode->getId(), strEquation);
    DynamicalEquation* nodeEquation = newNetwork->getNodeDynamicalEquation(newNode->getId());
    std::vector<Node*> nodes = newNetwork->getNodeNeighbors(newNode->getId());
    std::map<int, Node*> nodesMap;
    for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
    {
      nodesMap[(*itNode)->getId()] = *itNode;
    }
    nodeEquation->loadNodesToEquation(nodeEquation->getBaseCalculationNode(), nodesMap);
  } 
}

Node* NetworkModifier::chooseNode(Network* network)
{
  std::vector<Node*> nodes = network->getNodes();
  int randomIndex = rand()%static_cast<int>(nodes.size());
  return nodes[randomIndex];
}

Node* NetworkModifier::chooseNewNeighbor(Network* network, Node* node)
{
  std::vector<Node*> allNodes = network->getNodes();
  std::vector<Node*> neighbors = network->getNodeNeighbors(node->getId());
  std::vector<Node*> nonNeighbors;
  for(std::vector<Node*>::iterator itNode=allNodes.begin(); itNode != allNodes.end(); ++itNode)
  {
    if(std::find(neighbors.begin(), neighbors.end(), (*itNode)) == neighbors.end())
    {
      nonNeighbors.push_back((*itNode));
    }
  }

  int numberOfNonNeighbors = nonNeighbors.size();
  int randomIndex = rand()%static_cast<int>(numberOfNonNeighbors);

  return nonNeighbors[randomIndex];
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

void NetworkModifier::addEdge(Network* network, Node* node)
{
  Node* newNeighbor = chooseNewNeighbor(network, node);
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> insertLocations;
  getInsertLocations(baseCalcNode, insertLocations);
  if(insertLocations.size() == 0){return;}
  int randomIndex = rand()%static_cast<int>(insertLocations.size());
  CalculationNode* randomLocation = insertLocations[randomIndex];

  baseCalcNode = insertNodeAtLocation(baseCalcNode, randomLocation, newNeighbor);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
  int localId1 = network->getLocalId(node->getId());
  int localId2 = network->getLocalId(newNeighbor->getId());
  network->addEdge(localId1, localId2);
}

void NetworkModifier::removeEdge(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  Node* neighborToRemove = getNode_removeEdge(network, node);
  if(neighborToRemove == NULL){return;}

  std::vector<CalculationNode*> deleteList;
  getNodeOccurrences(baseCalcNode, deleteList, neighborToRemove->getId());

  for(std::vector<CalculationNode*>::iterator itCalc=deleteList.begin(); itCalc != deleteList.end(); ++itCalc)
  {
    CalculationNode* changingCalcNode = (*itCalc);
    baseCalcNode = specific_removeEdge(baseCalcNode, changingCalcNode);
  }

  nodeEquation->setBaseCalculationNode(baseCalcNode);
  int localId1 = network->getLocalId(node->getId());
  int localId2 = network->getLocalId(neighborToRemove->getId());
  network->removeEdge(localId1, localId2);
}

void NetworkModifier::addToOuterBlock(Network* network, Node* node)
{
  Node* reAddedNode = getNode_removeEdge(network, node);
  if(reAddedNode == NULL)
  {
    return;
  }
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> insertLocations;
  getOuterInsertLocations(baseCalcNode, insertLocations);
  int randomIndex = rand()%static_cast<int>(insertLocations.size());
  CalculationNode* randomLocation = insertLocations[randomIndex];

  baseCalcNode = insertNodeAtLocation(baseCalcNode, randomLocation, reAddedNode);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
}

void NetworkModifier::removeFromOuterBlock(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> outerElements;
  getOuterInsertLocations(baseCalcNode, outerElements);
  std::vector<CalculationNode*> outerIds;
  for(std::vector<CalculationNode*>::iterator itCalc=outerElements.begin(); itCalc != outerElements.end(); ++itCalc)
  {
    CalculationNode* currentCalcNode = (*itCalc);
    if(currentCalcNode->getType() == ID)
    {
      std::vector<CalculationNode*> nodeOccurrences;
      getNodeOccurrences(baseCalcNode, nodeOccurrences, currentCalcNode->getId());
      if(nodeOccurrences.size() > 1)
      {
	outerIds.push_back(currentCalcNode);	
      }
    }
  }
  if(outerIds.size() != 0)
  {
    int randomIndex = rand()%static_cast<int>(outerIds.size());
    CalculationNode* outerNodeToRemove = outerIds[randomIndex];   
    baseCalcNode = specific_removeEdge(baseCalcNode, outerNodeToRemove);
    nodeEquation->setBaseCalculationNode(baseCalcNode);
  }
}

void NetworkModifier::changeConstant(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();
  int nrOfConstants = numberOfType(baseCalcNode, CONSTANT);
  if(nrOfConstants == 0)
  {
    return;
  }
  int randomConstantElement = rand()%static_cast<int>(nrOfConstants);
  changeSpecifiedConstant(baseCalcNode, randomConstantElement, 0);
}

void NetworkModifier::changePlusToMultiply(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> locations;
  getLocations_changePlusToMultiply(baseCalcNode, locations);
  // std::cout<<locations.size()<<std::endl;
  if(locations.size() == 0){return;}
  int randomIndex = rand()%locations.size();
  CalculationNode* changingCalcNode = locations[randomIndex];

  baseCalcNode = specific_changePlusToMultiply(baseCalcNode, changingCalcNode);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
}

void NetworkModifier::changeMultiplyToPlus(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> locations;
  getLocations_changeMultiplyToPlus(baseCalcNode, locations);
  if(locations.size() == 0){return;}
  int randomIndex = rand()%locations.size();
  CalculationNode* changingCalcNode = locations[randomIndex];
  
  baseCalcNode = specific_changeMultiplyToPlus(baseCalcNode, changingCalcNode);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
}

//-----------------------------------
//--------getNode functions-----------
//-----------------------------------
Node* NetworkModifier::getNode_removeEdge(Network* network, Node* node)
{
  std::vector<Node*> neighbors = network->getNodeNeighbors(node->getId());
  if(neighbors.size() == 0)
  {
    return NULL;
  }
  int randomIndex = rand()%static_cast<int>(neighbors.size());
  return neighbors[randomIndex];
}


//-----------------------------------
//--------getLocations functions-----------
//-----------------------------------
void NetworkModifier::getLocations_changePlusToMultiply(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  bool pushBack = true;
  if(calcNode->left != NULL)
  {
    if(calcNode->left->getType() == CONSTANT)
    {
      pushBack = false;
    }
  }
  if(calcNode->right != NULL)
  {
    if(calcNode->right->getType() == CONSTANT)
    {
      pushBack = false;
    }
  }
  if((calcNode->getType() == MULTIPLY) && (pushBack))
  {
    locations.push_back(calcNode);
  }

  if(calcNode->left != NULL)
  {
    getInsertLocations(calcNode->left, locations);
  }
  if(calcNode->right != NULL)
  {
    getInsertLocations(calcNode->right, locations);
  }

  return;  
}


void NetworkModifier::getLocations_changeMultiplyToPlus(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  if(calcNode->getType() == MULTIPLY)
  {
    if(!((calcNode->left->getType() == ID) && (calcNode->right->getType() == CONSTANT)))
    {
      locations.push_back(calcNode);
    }
  }

  if(calcNode->left != NULL)
  {
    getInsertLocations(calcNode->left, locations);
  }
  if(calcNode->right != NULL)
  {
    getInsertLocations(calcNode->right, locations);
  }

  return;  
}

//-----------------------------------
//--------Specific change functions-----------
//-----------------------------------
CalculationNode* NetworkModifier::specific_removeEdge(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode)
{
  if(changingCalcNode == baseCalcNode)
  {
    delete changingCalcNode;
    baseCalcNode = NULL;
  }
  else
  {
    CalculationNode* parent = getParent(baseCalcNode, changingCalcNode);

    if(parent->getType() == MULTIPLY)
    {
      if((parent->left == changingCalcNode) && (parent->right->getType() == CONSTANT))
      {
	changingCalcNode = parent;
	if(parent == baseCalcNode)
	{
	  baseCalcNode = NULL;
	  delete changingCalcNode;
	  return baseCalcNode;
	}
	else
	{
	  parent = getParent(baseCalcNode, parent);
	}
      }
      else if((parent->right == changingCalcNode) && (parent->left->getType() == CONSTANT))
      {
	changingCalcNode = parent;
	if(parent == baseCalcNode)
	{
	  baseCalcNode = NULL;
	  delete changingCalcNode;
	  return baseCalcNode;
	}
	else
	{
	  parent = getParent(baseCalcNode, parent);
	}
      }
    }

    if(parent == baseCalcNode)
    {
      if(parent->left == changingCalcNode)
      {
	baseCalcNode = parent->right;
      }
      else if(parent->right == changingCalcNode)
      {
	baseCalcNode = parent->left;
      }
      
      delete changingCalcNode;
      parent->left = NULL;
      parent->right = NULL;
      delete parent;      
    }
    else
    {
      CalculationNode* grandParent = getParent(baseCalcNode, parent);
      if(grandParent->left == parent)
      {
	if(parent->left == changingCalcNode)
	{
	  grandParent->left = parent->right;
	}
	else if(parent->right == changingCalcNode)
	{
	  grandParent->left = parent->left;
	}
      }
      else if(grandParent->right == parent)
      {
	if(parent->left == changingCalcNode)
	{
	  grandParent->right = parent->right;
	}
	else if(parent->right == changingCalcNode)
	{
	  grandParent->right = parent->left;
	}
      }
      delete changingCalcNode;
      parent->left = NULL;
      parent->right = NULL;
      delete parent;
    }
  }
  
  return baseCalcNode;
}

CalculationNode* NetworkModifier::specific_changePlusToMultiply(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode)
{
  if(changingCalcNode == baseCalcNode)
  {
    CalculationNode* multiplyNode = new CNMultiply(changingCalcNode->left, changingCalcNode->right);
    baseCalcNode = multiplyNode;

    changingCalcNode->left = NULL;
    changingCalcNode->right = NULL;
    delete changingCalcNode;
  }
  else
  {
    CalculationNode* parentCalcNode = getParent(baseCalcNode, changingCalcNode);
    CalculationNode* multiplyNode = new CNMultiply(changingCalcNode->left, changingCalcNode->right);
    if(parentCalcNode->left == changingCalcNode)
    {
      parentCalcNode->left = multiplyNode;
    }
    else if(parentCalcNode->right == changingCalcNode)
    {
      parentCalcNode->right = multiplyNode;
    }

    changingCalcNode->left = NULL;
    changingCalcNode->right = NULL;
    delete changingCalcNode;
  }

  return baseCalcNode;
}

CalculationNode* NetworkModifier::specific_changeMultiplyToPlus(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode)
{
  if(changingCalcNode == baseCalcNode)
  {
    CalculationNode* addNode = new CNAdd(changingCalcNode->left, changingCalcNode->right);
    baseCalcNode = addNode;

    changingCalcNode->left = NULL;
    changingCalcNode->right = NULL;
    delete changingCalcNode;
  }
  else
  {
    CalculationNode* parentCalcNode = getParent(baseCalcNode, changingCalcNode);
    CalculationNode* addNode = new CNAdd(changingCalcNode->left, changingCalcNode->right);
    if(parentCalcNode->left == changingCalcNode)
    {
      parentCalcNode->left = addNode;
    }
    else if(parentCalcNode->right == changingCalcNode)
    {
      parentCalcNode->right = addNode;
    }

    changingCalcNode->left = NULL;
    changingCalcNode->right = NULL;
    delete changingCalcNode;
  }

  return baseCalcNode;
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
  if((calcNode->left == childCalcNode) || (calcNode->right == childCalcNode))
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

void NetworkModifier::getInsertLocations(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  if((calcNode->getType() != CONSTANT) && (calcNode->getType() != ID))
  {
    locations.push_back(calcNode);
  }

  if(calcNode->left != NULL)
  {
    getInsertLocations(calcNode->left, locations);
  }
  if(calcNode->right != NULL)
  {
    getInsertLocations(calcNode->right, locations);
  }

  return;
}

void NetworkModifier::getOuterInsertLocations(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  if((calcNode->getType() != CONSTANT) && (calcNode->getType() != ID))
  {
    locations.push_back(calcNode);
  }

  if((calcNode->left != NULL) && ((calcNode->getType() == ADD) || (calcNode->getType() == SUBSTRACT)))
  {
    getInsertLocations(calcNode->left, locations);
  }
  if((calcNode->right != NULL) && ((calcNode->getType() == ADD) || (calcNode->getType() == SUBSTRACT)))
  {
    getInsertLocations(calcNode->right, locations);
  }

  return;        
}

void NetworkModifier::getNodeOccurrences(CalculationNode* calcNode, std::vector<CalculationNode*> &locations, int nodeId)
{
  if(calcNode->getType() == ID)
  {
    if(calcNode->getId() == nodeId)
    {
      locations.push_back(calcNode);      
    }
  }

  if(calcNode->left != NULL)
  {
    getNodeOccurrences(calcNode->left, locations, nodeId);
  }
  if(calcNode->right != NULL)
  {
    getNodeOccurrences(calcNode->right, locations, nodeId);
  }

  return;        
}

CalculationNode* NetworkModifier::insertNodeAtLocation(CalculationNode* baseCalcNode, CalculationNode* calcNode, Node* node)
{
  CalculationNode* idNode = new CNId(node->getId());
  idNode->setNode(node);
  CalculationNode* constantNode = new CNConstant(rand()/static_cast<float>(RAND_MAX));
  CalculationNode* idConstMultiplyNode = new CNMultiply(constantNode, idNode);
  CalculationNode* addNode = new CNAdd(calcNode, idConstMultiplyNode);
  if(baseCalcNode == calcNode)
  {
    baseCalcNode = addNode;
  }
  else
  {
    CalculationNode* parent = getParent(baseCalcNode, calcNode);
    if(parent->left == calcNode)
    {
      parent->left = addNode;
    }
    else if(parent->right == calcNode)
    {
      parent->right = addNode;
    }
  }

  return baseCalcNode;
}
