#include "VectorFieldTransformImplementations.hh"
#include "Node.hh"
#include "Network.hh"
#include "MultilayerNetwork.hh"
#include "VectorField.hh"
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
  adjustVectorField(targetVectorField, isolatedVectorField, environmentalVectorField, parameters, currentState);

  delete multilayerNetwork;
  delete environmentalVectorField;
}

void adjustVectorField(VectorField* adjustedVectorField, VectorField* beginVectorField, VectorField* endVectorField, SimulationParameterContainer *parameters, std::vector<IdValuePair> &baseCoordinate)
{
  std::vector<VectorFieldPoint*> vectorFieldPoints = beginVectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    VectorFieldPoint* currentBeginPoint = *itPoint;
    std::vector<IdValuePair> currentCoordinate = currentBeginPoint->getCoordinate();
    std::vector<IdValuePair> beginDirection = currentBeginPoint->getDirection();

    double distance = coordinateDistance(baseCoordinate, currentCoordinate);

    // VectorFieldPoint* currentEndPoint = findCorrespondingPoint(currentBeginPoint, endVectorField);
    VectorFieldPoint* currentEndPoint = endVectorField->getPoint(currentCoordinate);
    std::vector<IdValuePair> endDirection = currentEndPoint->getDirection();
    std::vector<IdValuePair> adjustedDirection;
    for(std::vector<IdValuePair>::iterator itDir=beginDirection.begin(); itDir != beginDirection.end(); ++itDir)
    {
      int id = itDir->mId;
      double beginDirectionValue = itDir->mValue;
      double endDirectionValue = getValueForId(endDirection, id);
      double directionDifference = endDirectionValue-beginDirectionValue;
      double adjustmentMultiplier = parameters->adjustmentDistanceFunction(parameters->adjustmentFactor, distance);
      double adjustedDirectionValue = beginDirectionValue+adjustmentMultiplier*directionDifference;
      adjustedDirection.push_back(IdValuePair(id, adjustedDirectionValue));
    }
    adjustedVectorField->addPoint(currentCoordinate, adjustedDirection);
  }
}
