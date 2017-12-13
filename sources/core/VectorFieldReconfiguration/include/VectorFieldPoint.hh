#ifndef VECTORFIELDPOINT_HH
#define VECTORFIELDPOINT_HH

#include "IdValuePair.hh"
#include <iostream>
#include <vector>

class VectorFieldPoint
{
public:
  VectorFieldPoint(std::vector<IdValuePair> coordinate, std::vector<IdValuePair> direction)
  {
    mCoordinate = coordinate;
    mDirection = direction;
  }
  ~VectorFieldPoint(){};

  std::vector<IdValuePair> getCoordinate()
  {
    return mCoordinate;
  }
  
  std::vector<IdValuePair> getDirection()
  {
    return mDirection;
  }

  friend std::ostream& operator<<(std::ostream& os, const VectorFieldPoint &vectorFieldPoint);
private:
  std::vector<IdValuePair> mCoordinate;
  std::vector<IdValuePair> mDirection;
};

#endif
