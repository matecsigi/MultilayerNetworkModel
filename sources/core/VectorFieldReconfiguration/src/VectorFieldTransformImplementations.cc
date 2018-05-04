#include "VectorFieldTransformImplementations.hh"
#include "VectorFieldSchemes.hh"
#include "NetworkUtilityFunctions.hh"
#include "UtilityFunctions.hh"

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

  delete multilayerNetwork;
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
