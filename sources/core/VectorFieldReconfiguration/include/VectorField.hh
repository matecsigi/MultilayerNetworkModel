#ifndef VECTORFIELD_HH
#define VECTORFIELD_HH

#include "VectorFieldPoint.hh"
#include <vector>

class VectorField
{
public:
  VectorField();
  ~VectorField();

  void addPoint(std::vector<IdValuePair> &coordinate, std::vector<IdValuePair> &direction);
  VectorFieldPoint* getPoint(std::vector<IdValuePair> &coordinate);
  std::vector<VectorFieldPoint*> getVectorFieldPoints();

  double getDistanceFrom(VectorField* vectorField);

  friend std::ostream& operator<<(std::ostream& os, const VectorField &vectorField);
private:
  std::vector<VectorFieldPoint*> mVectorFieldPoints;
};

#endif
