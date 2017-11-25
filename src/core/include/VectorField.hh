#ifndef VECTORFIELD_HH
#define VECTORFIELD_HH

#include "VectorFieldPoint.hh"
#include <vector>

class VectorField
{
public:
  VectorField(){};
  ~VectorField(){};

  void addPoint(std::map<int, double> coordinate, std::map<int, double> direction)
  {
    mVectorFieldPoints.push_back(VectorFieldPoint(coordinate, direction));
  }
private:
  std::vector<VectorFieldPoint> mVectorFieldPoints;
};

#endif
