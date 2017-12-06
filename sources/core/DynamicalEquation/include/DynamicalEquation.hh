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
  double evaluateAtState(std::map<int, double> startingState);
  void ODEcalculator(const state_type &x, state_type &dxdt, double t);
  void ODEcalculatorAtState(const state_type &x, state_type &dxdt, double t, std::map<int, double> startingState);

  /**
     Used only for testing purposes.
     For all ID type CalculationNodes in the equation compares the id field to the 
     assignes node's id.
   */
  bool testNodeIds(void) const;

  CalculationNode* getBaseCalculationNode(void);
  void setBaseCalculationNode(CalculationNode* baseCalcNode);

  void loadEquation(std::string strEquation);
  void loadNodesToEquation(CalculationNode* calcPtr, std::map<int, Node*>& nodesMap);
  std::string toString() const;
private:
  CalculationNode* mBaseCalculationNode;
};

#endif