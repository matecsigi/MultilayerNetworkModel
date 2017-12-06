#include "DynamicalEquation.hh"
#include "interpreter.h"
#include "scanner.h"
#include "parser.hpp"
#include <iostream>
#include <sstream>

DynamicalEquation::DynamicalEquation()
{
  mBaseCalculationNode = NULL;
}

DynamicalEquation::~DynamicalEquation()
{
  delete mBaseCalculationNode;
}

double DynamicalEquation::evaluate()
{
  return mBaseCalculationNode->evaluate();
}

double DynamicalEquation::evaluateAtState(std::map<int, double> startingState)
{
  return mBaseCalculationNode->evaluateAtState(startingState);
}

void DynamicalEquation::ODEcalculator(const state_type &x, state_type &dxdt, double t)
{
  dxdt[0] = evaluate();
}

void DynamicalEquation::ODEcalculatorAtState(const state_type &x, state_type &dxdt, double t, std::map<int, double> startingState)
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
}

void DynamicalEquation::loadEquation(std::string strEquation)
{
  if(strEquation.empty())
  {
    mBaseCalculationNode = NULL;
    return;
  }
  EquationParser::Interpreter i;
  // std::string myString = "1+(1+ID1)";
  // std::istringstream myStream(myString);
  std::istringstream myStream(strEquation);
  i.switchInputStream(&myStream);
  i.parse();
  mBaseCalculationNode = i.getBaseCalculationNode();
}

void DynamicalEquation::loadNodesToEquation(CalculationNode* calcPtr, std::map<int, Node*>& nodesMap)
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
