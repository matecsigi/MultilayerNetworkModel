#include "CalculationNode.hh"
#include "Node.hh"

bool CNId::testNodeIds() const
{
  return (id == mNode->getId());
}
