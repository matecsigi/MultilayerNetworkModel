#ifndef DYNAMICALEQUATION_HH
#define DYNAMICALEQUATION_HH

#include <iostream>
#include <string>

// #include "CalculationNode.hh"

class DynamicalEquation
{
public:
  DynamicalEquation();
  ~DynamicalEquation();

  void loadEquation(std::string strEquation);
private:
  // CalculationNode* mBaseCalculationNode;
};

#endif
