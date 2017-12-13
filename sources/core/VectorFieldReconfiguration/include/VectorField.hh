#ifndef VECTORFIELD_HH
#define VECTORFIELD_HH

#include "VectorFieldPoint.hh"
#include <vector>

class VectorField
{
public:
  VectorField();
  ~VectorField();

  void addPoint(std::vector<IdValuePair> coordinate, std::vector<IdValuePair> direction);
  std::vector<VectorFieldPoint*> getVectorFieldPoints();
  double getDistanceFrom(VectorField* vectorField);
  std::vector<IdValuePair> getDirectionForCoordinate(std::vector<IdValuePair> coordinate);

  friend std::ostream& operator<<(std::ostream& os, const VectorField &vectorField);
private:
  std::vector<VectorFieldPoint*> mVectorFieldPoints;
};

#endif
