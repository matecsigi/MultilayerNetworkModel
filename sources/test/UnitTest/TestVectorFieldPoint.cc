#include <boost/test/unit_test.hpp>
#include "VectorFieldPoint.hh"

BOOST_AUTO_TEST_SUITE(TestVectorFieldPoint)

BOOST_AUTO_TEST_CASE(VectorFieldPointIdDuplication)
{
  std::vector<IdValuePair> coordinate;
  std::vector<IdValuePair> direction;
  for(int i=0; i<10; ++i)
  {
    coordinate.push_back(IdValuePair(i, i));
    direction.push_back(IdValuePair(i, 1/(double)i));
  }

  // Duplicating Id - uncommenting yields error
  // coordinate.push_back(IdValuePair(1, 2));
  // direction.push_back(IdValuePair(1, 1));

  // Different coordinate and direction dimension - uncommenting yields error
  // coordinate.push_back(IdValuePair(100, 2));

  // Different coordinate and dimension ids - uncommenting yields error
  // coordinate.push_back(IdValuePair(100, 2));
  // direction.push_back(IdValuePair(101, 1));

  int dimensionOriginal = coordinate.size();

  VectorFieldPoint *point = new VectorFieldPoint(coordinate, direction);
  int dimensionNew = point->getDimension();

  BOOST_CHECK_MESSAGE(dimensionOriginal == dimensionNew, "Possible to duplicate id in VectorFieldPoint.");

  delete point;
}

BOOST_AUTO_TEST_SUITE_END()
