#ifndef VECTORFIELDPOINT_HH
#define VECTORFIELDPOINT_HH

#include <map>

class VectorFieldPoint
{
public:
  VectorFieldPoint(std::map<int, double> coordinate, std::map<int, double> direction)
  {
    mCoordinate = coordinate;
    mDirection = direction;
  }
  ~VectorFieldPoint(){};

  std::map<int, double> getCoordinate()
  {
    return mCoordinate;
  }
  
  std::map<int, double> getDirection()
  {
    return mDirection;
  }

private:
  std::map<int, double> mCoordinate;
  std::map<int, double> mDirection;
};

#endif
