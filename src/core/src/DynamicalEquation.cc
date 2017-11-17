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

void DynamicalEquation::loadEquation(std::string strEquation)
{
  EzAquarii::Interpreter i;
  // std::string myString = "1+(1+ID1)";
  // std::istringstream myStream(myString);
  std::istringstream myStream(strEquation);
  i.switchInputStream(&myStream);
  i.parse();
  mBaseCalculationNode = i.getBaseCalculationNode();
}

std::string DynamicalEquation::toString() const
{
  std::string strEquation;
  strEquation.clear();
  strEquation.append(mBaseCalculationNode->toString());
  return strEquation;
}
