#include "VectorFieldPoint.hh"
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const VectorFieldPoint &vectorFieldPoint)
{
  std::map<int, double> coordinate = vectorFieldPoint.mCoordinate;
  std::map<int, double> direction = vectorFieldPoint.mDirection;
  std::vector<int> keys;
  for(std::map<int, double>::iterator itCord=coordinate.begin(); itCord != coordinate.end(); ++itCord)
  {
    keys.push_back(itCord->first);
  }

  os<<"{";
  for(std::vector<int>::iterator itKey=keys.begin(); itKey != keys.end(); ++itKey)
  {
    os<<coordinate[(*itKey)]<<" ";
  }
  os<<"}";
  os<<"{";
  for(std::vector<int>::iterator itKey=keys.begin(); itKey != keys.end(); ++itKey)
  {
    os<<direction[(*itKey)]<<" ";
  }
  os<<"}";

  return os;
}
