#ifndef DYNAMICALEQUATION_HH
#define DYNAMICALEQUATION_HH

#include <iostream>
#include <string>

#include "CalculationNode.hh"

class DynamicalEquation
{
public:
  DynamicalEquation();
  ~DynamicalEquation();

  double evaluate();

  void loadEquation(std::string strEquation);
  std::string toString() const;
private:
  CalculationNode* mBaseCalculationNode;
};

#endif
