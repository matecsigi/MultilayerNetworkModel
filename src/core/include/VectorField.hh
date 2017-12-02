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
  VectorField();
  ~VectorField();

  void addPoint(std::map<int, double> coordinate, std::map<int, double> direction);
  std::vector<VectorFieldPoint*> getVectorFieldPoints();
  double getDistanceFrom(VectorField* vectorField);
  std::map<int, double> getDirectionForCoordinate(std::map<int, double> coordinate);

  friend std::ostream& operator<<(std::ostream& os, const VectorField &vectorField);
private:
  std::vector<VectorFieldPoint*> mVectorFieldPoints;
};

#endif
