#include "CalculationNode.hh"
#include "Node.hh"

double CNId::evaluate() const
{
  return mNode->getPreviousState();
}

double CNId::evaluateAtState(std::vector<IdValuePair> &startingState) const
{
  return getValueForId(startingState, mNode->getId());
}

bool CNId::testNodeIds() const
{
  return (id == mNode->getId());
}
