#include "VectorField.hh"
#include "VectorFieldClassUtility.hh"
#include "UtilityFunctions.hh"
#include <math.h>
#include <iostream>
#include <algorithm>

VectorField::VectorField()
{
}

VectorField::~VectorField()
{
  for(std::vector<VectorFieldPoint*>::iterator itPoint=mVectorFieldPoints.begin(); itPoint != mVectorFieldPoints.end(); ++itPoint)
  {
    delete (*itPoint);
  }
}

void VectorField::addPoint(std::vector<IdValuePair> &coordinate, std::vector<IdValuePair> &direction)
{
  if(mVectorFieldPoints.size() > 0)
  {
    VectorFieldPoint *referencePoint = mVectorFieldPoints[0];
    std::vector<IdValuePair> referenceCoordinate = referencePoint->getCoordinate();
    std::vector<int> referenceIds = getIds(referenceCoordinate);
    std::vector<int> ids = getIds(coordinate);

    if(ids.size() != referenceIds.size())
    {
      std::cout<<"Error: VectorFieldPoint dimensions don't match in VectorField."<<std::endl;
      exit(EXIT_FAILURE);
    }

    std::sort(referenceIds.begin(), referenceIds.end());
    std::sort(ids.begin(), ids.end());
    if(ids != referenceIds)
    {
      std::cout<<"Error: VectorFieldPoint coordinate ids don't match in VectorField"<<std::endl;
      exit(EXIT_FAILURE);
    }
  }

  VectorFieldPoint *point = getPoint(coordinate);
  if(point != NULL)
  {
    point->setDirection(direction);
  }
  else
  {
    mVectorFieldPoints.push_back(new VectorFieldPoint(coordinate, direction));
  }
}

VectorFieldPoint* VectorField::getPoint(std::vector<IdValuePair> &coordinate)
{
  for(std::vector<VectorFieldPoint*>::iterator itPoint=mVectorFieldPoints.begin(); itPoint != mVectorFieldPoints.end(); ++itPoint)
  {
    bool equal = true;
    std::vector<IdValuePair> currentCoordinate = (*itPoint)->getCoordinate();
    for(std::vector<IdValuePair>::iterator itCor=coordinate.begin(); itCor != coordinate.end(); ++itCor)
    {
      int key = itCor->mId;
      double value = itCor->mValue;
      if(getValueForId(currentCoordinate, key) != value)
      {
	equal = false;
	break;
      }
    }
    if(equal == true)
    {
      return (*itPoint);
    }
  }

  return NULL;
}

std::vector<VectorFieldPoint*> VectorField::getVectorFieldPoints()
{
  return mVectorFieldPoints;
}

double VectorField::getDistanceFrom(VectorField* vectorField)
{
  double distance = 0;
  int numberOfPoints = mVectorFieldPoints.size();

  std::vector<VectorFieldPoint*> vectorFieldPoints1 = mVectorFieldPoints;
  std::vector<VectorFieldPoint*> vectorFieldPoints2 = vectorField->getVectorFieldPoints();

  if((vectorFieldPoints1.size() == 0) || (vectorFieldPoints2.size() == 0))
  {
    std::cout<<"Error: Distance calculation for VectorField with 0 points."<<std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout<<"===VectorField1==="<<std::endl<<*this;
  std::cout<<"===VectorField2==="<<std::endl<<*vectorField;

  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints1.begin(); itPoint != vectorFieldPoints1.end(); ++itPoint)
  {
    VectorFieldPoint* currentPoint = (*itPoint);
    std::vector<IdValuePair> coordinate = currentPoint->getCoordinate();
    std::vector<IdValuePair> direction1 = currentPoint->getDirection();

    // std::cout<<"check 1"<<std::endl;
    VectorFieldPoint *currentPoint2 = vectorField->getPoint(coordinate);

    if(currentPoint2 == NULL)
    {
      std::cout<<"Error: VectorFieldPoints don't have matching coordinates in VectorField::distanceFrom"<<std::endl;
      exit(EXIT_FAILURE);
    }
    // std::cout<<"check2, currentPoint2="<<currentPoint2<<std::endl;
    std::vector<IdValuePair> direction2 = currentPoint2->getDirection();
    // std::cout<<"check 3"<<std::endl;

    double pointDistance = 0;
    for(std::vector<IdValuePair>::iterator itDir=direction1.begin(); itDir != direction1.end(); ++itDir)
    {
      int key = itDir->mId;
      double value1 = itDir->mValue;
      double value2 = getValueForId(direction2, key);
      double localDifference = fabs(value1-value2);
      pointDistance += localDifference*localDifference;
    }
    distance += sqrt(pointDistance);
  }
  distance = distance/(double)numberOfPoints;

  return distance;
}

std::ostream& operator<<(std::ostream& os, const VectorField &vectorField)
{
  std::vector<VectorFieldPoint*> vectorFieldPoints = vectorField.mVectorFieldPoints;
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    os<<*(*itPoint)<<std::endl;
  }
  
  return os;
}

