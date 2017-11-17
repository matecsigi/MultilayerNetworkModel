#ifndef DYNAMICALEQUATION_HH
#define DYNAMICALEQUATION_HH

#include <iostream>
#include <string>
#include <map>

#include "CalculationNode.hh"

class Node;
class Network;

class DynamicalEquation
{
public:
  DynamicalEquation();
  ~DynamicalEquation();

  double evaluate();

  CalculationNode* getBaseCalculationNode(void);

  void loadEquation(std::string strEquation);
  void loadNodesToEquation(CalculationNode* calcPtr, std::map<int, Node*>& nodesMap);
  std::string toString() const;
private:
  CalculationNode* mBaseCalculationNode;
};

#endif
