#include "VectorField.hh"
#include <math.h>
#include <iostream>

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

void VectorField::addPoint(std::map<int, double> coordinate, std::map<int, double> direction)
{
  mVectorFieldPoints.push_back(new VectorFieldPoint(coordinate, direction));
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

  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints1.begin(); itPoint != vectorFieldPoints1.end(); ++itPoint)
  {
    VectorFieldPoint* currentPoint = (*itPoint);
    std::map<int, double> coordinate = currentPoint->getCoordinate();
    std::map<int, double> direction1 = currentPoint->getDirection();
    std::map<int, double> direction2 = vectorField->getDirectionForCoordinate(coordinate);
    for(std::map<int, double>::iterator itDir=direction1.begin(); itDir != direction1.end(); ++itDir)
    {
      int key = itDir->first;
      double value1 = itDir->second;
      double value2 = direction2[key];
      double localDifference = fabs(value1-value2);
      //std::cout<<"  -localDiff="<<value1<<"-"<<value2<<std::endl;
      distance += localDifference;
    }
  }
  distance = distance/numberOfPoints;
  //  std::cout<<"distance="<<distance<<std::endl;
  return distance;
}

std::map<int, double> VectorField::getDirectionForCoordinate(std::map<int, double> coordinate)
{
  for(std::vector<VectorFieldPoint*>::iterator itPoint=mVectorFieldPoints.begin(); itPoint != mVectorFieldPoints.end(); ++itPoint)
  {
    bool equal = true;
    std::map<int, double> currentCoordinate = (*itPoint)->getCoordinate();
    for(std::map<int, double>::iterator itCord=coordinate.begin(); itCord != coordinate.end(); ++itCord)
    {
      int key = itCord->first;
      double value = itCord->second;
      if(currentCoordinate[key] != value)
      {
	equal = false;
      }
    }
    if(equal == true)
    {
      return (*itPoint)->getDirection();
    }
  }
  std::map<int, double> defaultReturn;
  return defaultReturn;
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

