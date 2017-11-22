#ifndef DYNAMICALEQUATION_HH
#define DYNAMICALEQUATION_HH

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "CalculationNode.hh"

typedef std::vector<double> state_type;

class Node;
class Network;

class DynamicalEquation
{
public:
  DynamicalEquation();
  ~DynamicalEquation();

  double evaluate();
  void ODEcalculator(const state_type &x, state_type &dxdt, double t);

  bool testNodeIds(void) const;

  CalculationNode* getBaseCalculationNode(void);

  void loadEquation(std::string strEquation);
  void loadNodesToEquation(CalculationNode* calcPtr, std::map<int, Node*>& nodesMap);
  std::string toString() const;
private:
  CalculationNode* mBaseCalculationNode;
};

#endif
