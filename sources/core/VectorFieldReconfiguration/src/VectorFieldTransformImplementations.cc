#include "VectorFieldTransformImplementations.hh"
#include "VectorFieldSchemes.hh"
#include "NetworkUtilityFunctions.hh"

void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, Node* node)
{
  std::vector<IdValuePair> directionInLowerNetwork = calculateLowerNetworkDirection(node);
  std::vector<IdValuePair> directionInHigherNetworks = calculateHigherNetworksDirection(node);

  //could by some more complicated expression
  double assignedNodeChange = getValueForId(directionInHigherNetworks, node->getId());

  std::vector<VectorFieldPoint*> vectorFieldPoints = currentVectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    VectorFieldPoint* currentPoint = (*itPoint);
    std::vector<IdValuePair> coordinate = currentPoint->getCoordinate();
    std::vector<IdValuePair> direction = currentPoint->getDirection();
    std::vector<IdValuePair> newDirection;
    for(std::vector<IdValuePair>::iterator itDir=direction.begin(); itDir != direction.end(); itDir++)
    {
      int key = itDir->mId;
      double currentDirectionValue = itDir->mValue;
      double value = currentDirectionValue+currentDirectionValue*getValueForId(directionInLowerNetwork, key)*assignedNodeChange;
      setValueForId(newDirection, key, value);
    }
    targetVectorField->addPoint(coordinate, newDirection);
  }
}

void sumVectorFieldTransform(VectorField* targetVectorField, VectorField* currentVectorField, Node* node)
{
  std::vector<IdValuePair> directionInLowerNetwork = calculateLowerNetworkDirection(node);
  std::vector<IdValuePair> directionInHigherNetworks = calculateHigherNetworksDirection(node);

  double sumNodeChange = 0;
  for(std::vector<IdValuePair>::iterator itDir=directionInHigherNetworks.begin(); itDir != directionInHigherNetworks.end(); ++itDir)
  {
    sumNodeChange += itDir->mValue;
  }
  // std::cout<<"sumNodeChange: "<<node->getId()<<"->"<<sumNodeChange<<std::endl;

  std::vector<VectorFieldPoint*> vectorFieldPoints = currentVectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    VectorFieldPoint* currentPoint = (*itPoint);
    std::vector<IdValuePair> coordinate = currentPoint->getCoordinate();
    std::vector<IdValuePair> direction = currentPoint->getDirection();
    std::vector<IdValuePair> newDirection;
    for(std::vector<IdValuePair>::iterator itDir=direction.begin(); itDir != direction.end(); itDir++)
    {
      int key = itDir->mId;
      double currentDirectionValue = itDir->mValue;
      double value = currentDirectionValue+currentDirectionValue*getValueForId(directionInLowerNetwork, key)*sumNodeChange;
      setValueForId(newDirection, key, value);
    }
    targetVectorField->addPoint(coordinate, newDirection);
  }  
}

void weightNodeVectorFieldTransform(VectorField* targetVectorField, VectorField* currentVectorField, Node* node)
{
  std::vector<IdValuePair> directionInLowerNetwork = calculateLowerNetworkDirection(node);
  std::vector<IdValuePair> directionInHigherNetworks = calculateHigherNetworksDirection(node);

  double sumNodeChange = 0;
  for(std::vector<IdValuePair>::iterator itDir=directionInHigherNetworks.begin(); itDir != directionInHigherNetworks.end(); ++itDir)
  {
    if(itDir->mId == node->getId())
    {
      sumNodeChange += 10*itDir->mValue;
    }
    else
    {
      sumNodeChange += itDir->mValue;
    }
  }

  std::vector<VectorFieldPoint*> vectorFieldPoints = currentVectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    VectorFieldPoint* currentPoint = (*itPoint);
    std::vector<IdValuePair> coordinate = currentPoint->getCoordinate();
    std::vector<IdValuePair> direction = currentPoint->getDirection();
    std::vector<IdValuePair> newDirection;
    for(std::vector<IdValuePair>::iterator itDir=direction.begin(); itDir != direction.end(); itDir++)
    {
      int key = itDir->mId;
      double currentDirectionValue = itDir->mValue;
      double value = currentDirectionValue+currentDirectionValue*getValueForId(directionInLowerNetwork, key)*sumNodeChange;
      setValueForId(newDirection, key, value);
    }
    targetVectorField->addPoint(coordinate, newDirection);
  }  
}

