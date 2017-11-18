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

void DynamicalEquation::loadEquation(std::string strEquation)
{
  if(strEquation.empty())
  {
    mBaseCalculationNode = NULL;
    return;
  }
  EzAquarii::Interpreter i;
  // std::string myString = "1+(1+ID1)";
  // std::istringstream myStream(myString);
  std::istringstream myStream(strEquation);
  i.switchInputStream(&myStream);
  i.parse();
  mBaseCalculationNode = i.getBaseCalculationNode();
}

void DynamicalEquation::loadNodesToEquation(CalculationNode* calcPtr, std::map<int, Node*>& nodesMap)
{
  std::cout<<calcPtr->getType()<<std::endl;
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
  }
  else if(calcPtr->left != NULL)
  {
    loadNodesToEquation(calcPtr->left, nodesMap);
  }
  else if(calcPtr->right != NULL)
  {
    loadNodesToEquation(calcPtr->right, nodesMap);
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
