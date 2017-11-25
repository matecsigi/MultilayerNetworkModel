#ifndef VECTORFIELD_HH
#define VECTORFIELD_HH

#include "VectorFieldPoint.hh"

class VectorField
{
public:
  VectorField(){};
  ~VectorField(){};
private:
  std::vector<VectorFieldPoint> mVectorFieldPoints;
};

#endif
