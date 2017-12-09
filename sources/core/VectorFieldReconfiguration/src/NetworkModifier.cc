#include "NetworkModifier.hh"
#include <iostream>
#include <algorithm>

NetworkModifier::NetworkModifier()
{
}

NetworkModifier::~NetworkModifier()
{
}

void NetworkModifier::modifyNetwork(Network* network, int numberOfChanges)
{
  for(int i=0; i<numberOfChanges; ++i)
  {
    Node* nodeToChange = chooseNode(network);
    ModificationType type = chooseType();

    // DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(nodeToChange->getId());
    // std::cout<<nodeEquation->toString()<<std::endl;

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
}

void NetworkModifier::copyNetwork(Network* oldNetwork, Network* newNetwork)
{
  newNetwork->removeAllEdges();
  
  std::vector<Node*> nodes = oldNetwork->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* oldNode = (*itNode);
    if(newNetwork->getNodeById(oldNode->getId()) == NULL)
    {
      newNetwork->addNode(oldNode->getId());      
    }
  }

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* oldNode = (*itNode);
    std::vector<Node*> neighbors = oldNetwork->getNodeNeighbors(oldNode->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      Node* oldNeighbor = (*itNei);
      newNetwork->addEdge(oldNode->getId(), oldNeighbor->getId());
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
    nodes.push_back(newNode);
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
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  Node* newNeighbor = getNode_addEdge(network, node);
  if(newNeighbor == NULL){return;}

  std::vector<CalculationNode*> locations;
  getLocations_addEdge(baseCalcNode, locations);
  if(locations.size() == 0){return;}
  int randomIndex = rand()%locations.size();
  CalculationNode* changingCalcNode = locations[randomIndex];

  baseCalcNode = specific_addEdge(baseCalcNode, changingCalcNode, newNeighbor);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
  network->addEdge(node->getId(), newNeighbor->getId());
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
  network->removeEdge(node->getId(), neighborToRemove->getId());
}


void NetworkModifier::addToOuterBlock(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  Node* reAddedNode = getNode_removeEdge(network, node);
  if(reAddedNode == NULL){return;}

  std::vector<CalculationNode*> locations;
  getLocations_addToOuterBlock(baseCalcNode, locations);
  if(locations.size() == 0){return;}
  int randomIndex = rand()%locations.size();
  CalculationNode* changingCalcNode = locations[randomIndex];  

  baseCalcNode = specific_addEdge(baseCalcNode, changingCalcNode, reAddedNode);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
}


void NetworkModifier::removeFromOuterBlock(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> locations;
  getLocations_removeFromOuterBlock(baseCalcNode, baseCalcNode, locations);
  if(locations.size() == 0){return;}
  int randomIndex = rand()%locations.size();
  CalculationNode* changingCalcNode = locations[randomIndex];  

  baseCalcNode = specific_removeEdge(baseCalcNode, changingCalcNode);
  nodeEquation->setBaseCalculationNode(baseCalcNode);
}


void NetworkModifier::changeConstant(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> locations;
  getLocations_changeConstant(baseCalcNode, locations);
  if(locations.size() == 0){return;}
  int randomIndex = rand()%locations.size();
  CalculationNode* changingCalcNode = locations[randomIndex];

  specific_changeConstant(changingCalcNode);
}


void NetworkModifier::changePlusToMultiply(Network* network, Node* node)
{
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(node->getId());
  CalculationNode* baseCalcNode = nodeEquation->getBaseCalculationNode();

  std::vector<CalculationNode*> locations;
  getLocations_changePlusToMultiply(baseCalcNode, locations);
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
Node* NetworkModifier::getNode_addEdge(Network* network, Node* node)
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
  if(nonNeighbors.size() == 0)
  {
    return NULL;
  }

  int randomIndex = rand()%static_cast<int>(nonNeighbors.size());

  return nonNeighbors[randomIndex];
}

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
void NetworkModifier::getLocations_addEdge(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  if((calcNode->getType() != CONSTANT) && (calcNode->getType() != ID))
  {
    locations.push_back(calcNode);
  }

  if(calcNode->left != NULL)
  {
    getLocations_addEdge(calcNode->left, locations);
  }
  if(calcNode->right != NULL)
  {
    getLocations_addEdge(calcNode->right, locations);
  }

  return;  
}

void NetworkModifier::getLocations_addToOuterBlock(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  if((calcNode->getType() != CONSTANT) && (calcNode->getType() != ID))
  {
    locations.push_back(calcNode);
  }

  if((calcNode->left != NULL) && ((calcNode->getType() == ADD) || (calcNode->getType() == SUBSTRACT)))
  {
    getLocations_addToOuterBlock(calcNode->left, locations);
  }
  if((calcNode->right != NULL) && ((calcNode->getType() == ADD) || (calcNode->getType() == SUBSTRACT)))
  {
    getLocations_addToOuterBlock(calcNode->right, locations);
  }

  return;          
}

void NetworkModifier::getLocations_removeFromOuterBlock(CalculationNode* baseCalcNode, CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  if(calcNode->getType() == ID)
  {
    std::vector<CalculationNode*> nodeOccurrences;
    getNodeOccurrences(baseCalcNode, nodeOccurrences, calcNode->getId());
    if(nodeOccurrences.size() > 1)
    {
      locations.push_back(calcNode);
    }
  }

  if((calcNode->left != NULL) && ((calcNode->getType() == ADD) || (calcNode->getType() == SUBSTRACT)))
  {
    getLocations_removeFromOuterBlock(baseCalcNode, calcNode->left, locations);
  }
  if((calcNode->right != NULL) && ((calcNode->getType() == ADD) || (calcNode->getType() == SUBSTRACT)))
  {
    getLocations_removeFromOuterBlock(baseCalcNode, calcNode->right, locations);
  }

  return;          
}

void NetworkModifier::getLocations_changeConstant(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  if(calcNode->getType() == CONSTANT)
  {
    locations.push_back(calcNode);
  }

  if(calcNode->left != NULL)
  {
    getLocations_changeConstant(calcNode->left, locations);
  }
  if(calcNode->right != NULL)
  {
    getLocations_changeConstant(calcNode->right, locations);
  }

  return;    
}


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
  if((calcNode->getType() == ADD) && (pushBack))
  {
    locations.push_back(calcNode);
  }

  if(calcNode->left != NULL)
  {
    getLocations_changePlusToMultiply(calcNode->left, locations);
  }
  if(calcNode->right != NULL)
  {
    getLocations_changePlusToMultiply(calcNode->right, locations);
  }

  return;  
}


void NetworkModifier::getLocations_changeMultiplyToPlus(CalculationNode* calcNode, std::vector<CalculationNode*> &locations)
{
  bool pushBack = true;
  if(calcNode->getType() == MULTIPLY)
  {
    if((calcNode->left->getType() == ID) && (calcNode->right->getType() == CONSTANT))
    {
      pushBack = false;
    }
    else if((calcNode->right->getType() == ID) && (calcNode->left->getType() == CONSTANT))
    {
      pushBack = false;
    }
  }

  if((calcNode->getType() == MULTIPLY) && (pushBack == true))
  {
    locations.push_back(calcNode);
  }

  if(calcNode->left != NULL)
  {
    getLocations_changeMultiplyToPlus(calcNode->left, locations);
  }
  if(calcNode->right != NULL)
  {
    getLocations_changeMultiplyToPlus(calcNode->right, locations);
  }

  return;  
}

//-----------------------------------
//--------Specific change functions-----------
//-----------------------------------
CalculationNode* NetworkModifier::specific_addEdge(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode, Node* newNeighbor)
{
  CalculationNode* idNode = new CNId(newNeighbor->getId());
  idNode->setNode(newNeighbor);
  CalculationNode* constantNode = new CNConstant(rand()/static_cast<float>(RAND_MAX));
  CalculationNode* idConstMultiplyNode = new CNMultiply(constantNode, idNode);
  CalculationNode* addNode = new CNAdd(changingCalcNode, idConstMultiplyNode);
  if(baseCalcNode == changingCalcNode)
  {
    baseCalcNode = addNode;
  }
  else
  {
    CalculationNode* parent = getParent(baseCalcNode, changingCalcNode);
    if(parent->left == changingCalcNode)
    {
      parent->left = addNode;
    }
    else if(parent->right == changingCalcNode)
    {
      parent->right = addNode;
    }
  }

  return baseCalcNode;  
}

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

void NetworkModifier::specific_changeConstant(CalculationNode* changingCalcNode)
{
  double value = changingCalcNode->getValue();
  value += -1+2*((double)rand()/RAND_MAX);
  changingCalcNode->setValue(value);
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
