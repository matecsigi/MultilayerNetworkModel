#include "CalculationNode.hh"
#include "Node.hh"

double CNId::evaluate() const
{
  return mNode->getPreviousState();
}

double CNId::evaluateAtState(std::map<int, double> startingState) const
{
  return startingState[mNode->getId()];
}

bool CNId::testNodeIds() const
{
  return (id == mNode->getId());
}
