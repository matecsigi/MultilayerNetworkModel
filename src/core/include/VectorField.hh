#ifndef VECTORFIELD_HH
#define VECTORFIELD_HH

#include "VectorFieldPoint.hh"
#include <vector>
#include <map>

//int: node id
//double: coordinate
typedef std::map<int, double> Coordinate;

typedef std::map<int, std::pair<double, double>> FieldPoint;

class VectorField
{
public:
  VectorField(){};
  ~VectorField()
  {
    for(std::vector<VectorFieldPoint*>::iterator itPoint=mVectorFieldPoints.begin(); itPoint != mVectorFieldPoints.end(); ++itPoint)
    {
      delete (*itPoint);
    }
  }

  void addPoint(std::map<int, double> coordinate, std::map<int, double> direction)
  {
    mVectorFieldPoints.push_back(new VectorFieldPoint(coordinate, direction));
  }

  std::vector<VectorFieldPoint*> getVectorFieldPoints()
  {
    return mVectorFieldPoints;
  }
private:
  std::vector<VectorFieldPoint*> mVectorFieldPoints;
};

#endif
