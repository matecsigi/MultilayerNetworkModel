#ifndef VECTORFIELDPOINT_HH
#define VECTORFIELDPOINT_HH

#include <vector>

class VectorFieldPoint
{
public:
  VectorFieldPoint(std::vector<double> coordinate, std::vector<double> direction)
  {
    mCoordinate = coordinate;
    mDirection = direction;
  }

  ~VectorFieldPoint(){};

private:
  std::vector<double> mCoordinate;
  std::vector<double> mDirection;
};

#endif
