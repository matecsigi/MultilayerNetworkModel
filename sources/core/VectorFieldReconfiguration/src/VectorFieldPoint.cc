#include "VectorFieldPoint.hh"
#include "UtilityFunctions.hh"
#include <iostream>
#include <vector>
#include <algorithm>

VectorFieldPoint::VectorFieldPoint(std::vector<IdValuePair> &coordinate, std::vector<IdValuePair> &direction)
{
  //Check for duplicated id
  std::vector<int> coordinateIds = getIds(coordinate);
  std::vector<int> directionIds = getIds(direction); 
  if(containsDuplicate(coordinateIds) || containsDuplicate(directionIds))
  {
    std::cout<<"Error: VectorFieldPoint coordinate or direction contains duplicated id."<<std::endl;
    exit(EXIT_FAILURE);
  }

  //Check if coordinate and direction dimensions match
  if(coordinateIds.size() != directionIds.size())
  {
    std::cout<<"Error: VectorFieldPoint coordinate and direction dimensions don't match."<<std::endl;
    exit(EXIT_FAILURE);
  }

  //check if coordinate and direction ids match
  std::sort(coordinateIds.begin(), coordinateIds.end());
  std::sort(directionIds.begin(), directionIds.end());
  if(coordinateIds != directionIds)
  {
    std::cout<<"Error: VectorFieldPoint coordinate and direction ids don't match."<<std::endl;
    exit(EXIT_FAILURE);
  }
  
  mCoordinate = coordinate;
  mDirection = direction;
}

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
