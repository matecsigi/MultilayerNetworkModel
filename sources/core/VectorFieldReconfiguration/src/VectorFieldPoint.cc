#include "VectorFieldPoint.hh"
#include "UtilityFunctions.hh"
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const VectorFieldPoint &vectorFieldPoint)
{
  std::vector<IdValuePair> coordinate = vectorFieldPoint.mCoordinate;
  std::vector<IdValuePair> direction = vectorFieldPoint.mDirection;
  std::vector<int> keys;
  for(std::vector<IdValuePair>::iterator itCor=coordinate.begin(); itCor != coordinate.end(); ++itCor)
  {
    keys.push_back(itCor->mId);
  }

  os<<"{";
  for(std::vector<int>::iterator itKey=keys.begin(); itKey != keys.end(); ++itKey)
  {
    os<<getValueForId(coordinate, (*itKey))<<" ";
  }
  os<<"}";
  os<<"{";
  for(std::vector<int>::iterator itKey=keys.begin(); itKey != keys.end(); ++itKey)
  {
    os<<getValueForId(direction, (*itKey))<<" ";
  }
  os<<"}";

  return os;
}
