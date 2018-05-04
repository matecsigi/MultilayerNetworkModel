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

  // Add points with different dimensions to a VectorField - uncommenting yields error
  // coordinate.push_back(IdValuePair(100, 1));
  // direction1.push_back(IdValuePair(100, 1));
  // vectorField->addPoint(coordinate, direction1);

  // Add points with different coordinate ids - uncommenting yields error
  // std::vector<IdValuePair> coordinate2;
  // std::vector<IdValuePair> direction3;
  // for(int i=0; i<10; ++i)
  // {
  //   coordinate2.push_back(IdValuePair(i+1, i));
  //   direction3.push_back(IdValuePair(i+1, 2*i));
  // }
  // vectorField->addPoint(coordinate2, direction3);

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

BOOST_AUTO_TEST_CASE(DistanceTest)
{
  VectorField *vectorField1 = new VectorField;
  VectorField *vectorField2 = new VectorField;

  for(int i=1; i<10; ++i)
  {
    std::vector<IdValuePair> coordinate1;
    std::vector<IdValuePair> direction1;
    std::vector<IdValuePair> coordinate2;
    std::vector<IdValuePair> direction2;
    for(int j=1; j<10; ++j)
    {
      coordinate1.push_back(IdValuePair(j, i*j));
      direction1.push_back(IdValuePair(j, i*j));
      coordinate2.push_back(IdValuePair(j, i*j));
      direction2.push_back(IdValuePair(j, (i*j)+2.1));
    }
    vectorField1->addPoint(coordinate1, direction1);
    vectorField2->addPoint(coordinate2, direction2);
  }
  
  vectorField1->getDistanceFrom(vectorField2);

  delete vectorField1;
  delete vectorField2;
}

BOOST_AUTO_TEST_SUITE_END()
