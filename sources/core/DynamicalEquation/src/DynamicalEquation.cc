#include "DynamicalEquation.hh"
#include "Network.hh"
#include "NetworkClassUtility.hh"
#include "NetworkModifier.hh"
#include "interpreter.h"
#include "scanner.h"
#include "parser.hpp"
#include <iostream>
#include <sstream>

DynamicalEquation::DynamicalEquation()
{
  mNetwork = NULL;
  mBaseCalculationNode = NULL;
}

DynamicalEquation::DynamicalEquation(Network* network)
{
  mNetwork = network;
  mBaseCalculationNode = NULL;
}

DynamicalEquation::~DynamicalEquation()
{
  delete mBaseCalculationNode;
}

double DynamicalEquation::getEdgeWeight(int nodeId)
{
  NetworkModifier networkModifier;
  double edgeWeight = networkModifier.getNodeMultiplier(mBaseCalculationNode, nodeId);
  return edgeWeight;
}

void DynamicalEquation::setEdgeWeight(int nodeId, double weight)
{
  NetworkModifier networkModifier;
  networkModifier.setNodeMultiplier(mBaseCalculationNode, nodeId, weight);  
}

double DynamicalEquation::evaluate()
{
  return mBaseCalculationNode->evaluate();
}

double DynamicalEquation::evaluateAtState(std::vector<IdValuePair> &startingState)
{
  // std::cout<<mBaseCalculationNode->toString()<<std::endl;
  return mBaseCalculationNode->evaluateAtState(startingState);
}

void DynamicalEquation::ODEcalculator(const state_type &x, state_type &dxdt, double t)
{
  dxdt[0] = evaluate();
}

void DynamicalEquation::ODEcalculatorAtState(const state_type &x, state_type &dxdt, double t, std::vector<IdValuePair> &startingState)
{
  dxdt[0] = evaluateAtState(startingState);
}

bool DynamicalEquation::testNodeIds(void) const
{
  if(mBaseCalculationNode != NULL)
  {
    return mBaseCalculationNode->testNodeIds();
  }
  return true;
}

CalculationNode* DynamicalEquation::getBaseCalculationNode(void)
{
  return mBaseCalculationNode;
}

void DynamicalEquation::setBaseCalculationNode(CalculationNode* baseCalcNode)
{
  mBaseCalculationNode = baseCalcNode;
  loadNodesToEquations(mNetwork);
}

void DynamicalEquation::loadEquation(DynamicalEquation* dynamicalEquation)
{
  mBaseCalculationNode = copyEquation(dynamicalEquation->getBaseCalculationNode(), mBaseCalculationNode);
  loadNodesToEquations(mNetwork);
}

CalculationNode* DynamicalEquation::copyEquation(CalculationNode* originalCalcNode, CalculationNode* newCalcNode)
{
  CalculationNode* left = NULL;
  CalculationNode* right = NULL;

  if(originalCalcNode->left != NULL)
  {
    left = copyEquation(originalCalcNode->left, left);
  }

  if(originalCalcNode->right != NULL)
  {
    right = copyEquation(originalCalcNode->right, right);
  }

  newCalcNode = copyCalcNode(originalCalcNode, left, right);

  return newCalcNode;
}

CalculationNode* DynamicalEquation::copyCalcNode(CalculationNode* calcNode, CalculationNode* left, CalculationNode* right)
{
  switch(calcNode->getType())
  {
  case CONSTANT:
    return new CNConstant(calcNode->evaluate());
  case ID:
    return new CNId(calcNode->getId());
  case NEGATE:
    return new CNNegate(right);
  case ADD:
    return new CNAdd(left, right);
  case SUBSTRACT:
    return new CNSubtract(left, right);
  case MULTIPLY:
    return new CNMultiply(left, right);
  case DIVIDE:
    return new CNDivide(left, right);
  case POWER:
    return new CNPower(left, right);
  default:
    return NULL;
  }
  return NULL;
}

void DynamicalEquation::loadEquationString(std::string strEquation)
{
  if(strEquation.empty())
  {
    mBaseCalculationNode = NULL;
    return;
  }
  EquationParser::Interpreter i;
  std::istringstream myStream(strEquation);
  i.switchInputStream(&myStream);
  i.parse();
  mBaseCalculationNode = i.getBaseCalculationNode();
  loadNodesToEquations(mNetwork);
}

void DynamicalEquation::loadNodesToEquation(CalculationNode* calcPtr, std::map<int, Node*> &nodesMap)
{
  if(calcPtr == NULL)
  {
    return;
  }
  if((calcPtr->left == NULL) && (calcPtr->right == NULL))
  {
    if(calcPtr->getType() == ID)
    {
      int nodeId = calcPtr->getId();
      calcPtr->setNode(nodesMap[nodeId]);
    }
    return;
  }
  else
  {
    if(calcPtr->left != NULL)
    {
      loadNodesToEquation(calcPtr->left, nodesMap);
    }
    if(calcPtr->right != NULL)
    {
      loadNodesToEquation(calcPtr->right, nodesMap);
    }
  }
}

void DynamicalEquation::reassignNodeIds(CalculationNode* calcPtr, std::map<int, int> &idMap)
{
  if(calcPtr == NULL)
  {
    return;
  }
  if((calcPtr->left == NULL) && (calcPtr->right == NULL))
  {
    if(calcPtr->getType() == ID)
    {
      int nodeId = calcPtr->getId();
      calcPtr->setId(idMap[nodeId]);
    }
    return;
  }
  else
  {
    if(calcPtr->left != NULL)
    {
      reassignNodeIds(calcPtr->left, idMap);
    }
    if(calcPtr->right != NULL)
    {
      reassignNodeIds(calcPtr->right, idMap);
    }
  }
}

std::string DynamicalEquation::toString() const
{
  std::string strEquation;
  strEquation.clear();
  if(mBaseCalculationNode != NULL)
  {
    strEquation.append(mBaseCalculationNode->toString());
    return strEquation;
  }
  return "";
}
