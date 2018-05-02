#ifndef VECTORFIELDPOINT_HH
#define VECTORFIELDPOINT_HH

#include "IdValuePair.hh"
#include <iostream>
#include <vector>

class VectorFieldPoint
{
public:
  VectorFieldPoint(std::vector<IdValuePair> &coordinate, std::vector<IdValuePair> &direction)
  {
    mCoordinate = coordinate;
    mDirection = direction;
  }
  ~VectorFieldPoint(){};

  std::vector<IdValuePair> getCoordinate()
  {
    return mCoordinate;
  }

  void setCoordinate(std::vector<IdValuePair> &coordinate)
  {
    mCoordinate = coordinate;
  }
  
  std::vector<IdValuePair> getDirection()
  {
    return mDirection;
  }

  void setDirection(std::vector<IdValuePair> &direction)
  {
    mDirection = direction;
  }

  int getDimension()
  {
    return mCoordinate.size();
  }

  friend std::ostream& operator<<(std::ostream& os, const VectorFieldPoint &vectorFieldPoint);
private:
  std::vector<IdValuePair> mCoordinate;
  std::vector<IdValuePair> mDirection;
};

#endif
