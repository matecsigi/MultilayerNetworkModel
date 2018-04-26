#include "NodeClassUtility.hh"
#include "Node.hh"

double getMaxValue(Node *node)
{
  double max = 0;
  double *values = NULL;
  node->getValues(values);
  for(int i=0; i<bufferSize; ++i)
  {
    if(values[i] > max)
    {
      max = values[i];
    }
  }
  return max;
}

double getMinValue(Node *node)
{
  double min = 0;
  double *values = NULL;
  node->getValues(values);
  for(int i=0; i<bufferSize; ++i)
  {
    if(values[i] < min)
    {
      min = values[i];
    }
  }
  return min;
}

bool nodesEqual(Node& node1, Node& node2)
{
  Network* networkAssigned1 = node1.getNetworkAssigned();
  Network* networkAssigned2 = node2.getNetworkAssigned();
  
  if((networkAssigned1 == NULL) && (networkAssigned2 == NULL))
  {
    return true;
  }
  else if((networkAssigned1 != NULL) && (networkAssigned2 == NULL))
  {
    return false;
  }
  else if((networkAssigned1 == NULL) && (networkAssigned2 != NULL))
  {
    return false;
  }
  else if(networkAssigned1->getId() != networkAssigned2->getId())
  {
    return false;
  }

  return true;
}
