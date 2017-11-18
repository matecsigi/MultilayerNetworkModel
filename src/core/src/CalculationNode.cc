#include "CalculationNode.hh"
#include "Node.hh"

double CNId::evaluate() const
{
  return mNode->getValue();
}

bool CNId::testNodeIds() const
{
  return (id == mNode->getId());
}
