#include <boost/test/unit_test.hpp>
#include "VectorField.hh"

BOOST_AUTO_TEST_SUITE(TestVectorField)

BOOST_AUTO_TEST_CASE(DuplicatedCoordinate)
{
  VectorField *vectorField = new VectorField;

  std::vector<IdValuePair> coordinate;
  std::vector<IdValuePair> direction1;
  std::vector<IdValuePair> direction2;
  for(int i=0; i<10; ++i)
  {
    coordinate.push_back(IdValuePair(i, i));
    direction1.push_back(IdValuePair(i, 2*i));
    direction2.push_back(IdValuePair(i, 3*i));
  }
  
  vectorField->addPoint(coordinate, direction1);
  vectorField->addPoint(coordinate, direction2);

  int numberOfPoints = (vectorField->getVectorFieldPoints()).size();
  BOOST_CHECK_MESSAGE(numberOfPoints == 1, "VectorField allows points with the same coordinate.");

  delete vectorField;
}

BOOST_AUTO_TEST_CASE(DistanceFromItself)
{
  VectorField *vectorField = new VectorField;

  std::vector<IdValuePair> coordinate;
  std::vector<IdValuePair> direction;
  for(int i=0; i<10; ++i)
  {
    coordinate.push_back(IdValuePair(i, i));
    direction.push_back(IdValuePair(i, i));
  }

  vectorField->addPoint(coordinate, direction);

  double selfDistance = vectorField->getDistanceFrom(vectorField);

  BOOST_CHECK_MESSAGE(selfDistance == 0, "VectorField distance from itself is not 0.");
  
  delete vectorField;
}

BOOST_AUTO_TEST_SUITE_END()
