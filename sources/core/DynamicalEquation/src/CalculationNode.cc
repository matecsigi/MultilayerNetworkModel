#include "CalculationNode.hh"
#include "Node.hh"
#include "UtilityFunctions.hh"

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

std::string CNId::toString() const
{
  std::string strCalcNode;
  strCalcNode.append("ID");
  strCalcNode.append(std::to_string(id));    
  return strCalcNode;
}
