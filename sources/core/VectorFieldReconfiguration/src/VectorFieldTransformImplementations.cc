#include "VectorFieldTransformImplementations.hh"
#include "VectorFieldSchemes.hh"
#include "NetworkUtilityFunctions.hh"

void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, Node* node, SimulationParameterContainer *parameters)
{
  VectorField* isolatedVectorField = currentVectorField;

  VectorField* environmentalVectorField = new VectorField();
  Network* network = node->getNetworkAssigned();
  std::vector<IdValuePair> currentState = network->getState();

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  Network* tmpNetwork = createEnvironmentalMultilayerNetwork(multilayerNetwork, network);

  gridAroundPointScheme2(environmentalVectorField, tmpNetwork, currentState, getEnvironmentalDirectionAtState);

  adjustVectorField(targetVectorField, isolatedVectorField, environmentalVectorField, parameters);

  delete environmentalVectorField;
}

void adjustVectorField(VectorField* adjustedVectorField, VectorField* beginVectorField, VectorField* endVectorField, SimulationParameterContainer *parameters)
{
  double distance = 0;

  std::vector<VectorFieldPoint*> vectorFieldPoints = beginVectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    VectorFieldPoint* currentBeginPoint = *itPoint;
    VectorFieldPoint* currentEndPoint = findCorrespondingPoint(currentBeginPoint, endVectorField);
    std::vector<IdValuePair> currentCoordinate = currentBeginPoint->getCoordinate();
    std::vector<IdValuePair> beginDirection = currentBeginPoint->getDirection();
    std::vector<IdValuePair> endDirection = currentEndPoint->getDirection();
    std::vector<IdValuePair> adjustedDirection;
    for(std::vector<IdValuePair>::iterator itDir=beginDirection.begin(); itDir != beginDirection.end(); ++itDir)
    {
      int id = itDir->mId;
      double beginDirectionValue = itDir->mValue;
      double endDirectionValue = getValueForId(endDirection, id);
      double directionDifference = endDirectionValue-beginDirectionValue;
      double adjustedDirectionValue = beginDirectionValue+parameters->adjustmentDistanceFunction(parameters->adjustmentFactor, distance)*directionDifference;
      adjustedDirection.push_back(IdValuePair(id, adjustedDirectionValue));
    }
    adjustedVectorField->addPoint(currentCoordinate, adjustedDirection);
  }
}

void calculateTargetVectorFieldOld(VectorField* targetVectorField, VectorField* currentVectorField, Node* node)
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

