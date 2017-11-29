#include "VectorFieldReconfigurationImpl.hh"
#include "VectorField.hh"
#include "VectorFieldSchemes.hh"
#include "NetworkModifier.hh"

void VectorFieldReconfigurationImpl::calculateVectorFieldReconfiguration()
{
  std::cout<<"VectorFieldReconfig node="<<mNode->getId()<<std::endl;
  Network* networkAssigned = mNode->getNetworkAssigned();
  if(networkAssigned == NULL)
  {
    return;
  }
  VectorField* currentVectorField = new VectorField();
  std::map<int, double> currentState = networkAssigned->getCurrentState();
  gridAroundPointScheme(currentVectorField, networkAssigned, currentState);

  VectorField* targetVectorField = new VectorField();
  std::map<int, double> directionInLowerNetwork = calculateLowerNetworkDirection();
  std::map<int, double> directionInHigherNetworks = calculateHigherNetworksDirection();
  calculateTargetVectorField(targetVectorField, currentVectorField, directionInLowerNetwork, directionInHigherNetworks);

  NetworkModifier* networkModifier = new NetworkModifier(networkAssigned);
  networkModifier->fitToVectorField(targetVectorField);

  delete currentVectorField;
  delete targetVectorField;
}

void VectorFieldReconfigurationImpl::calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, std::map<int, double> directionInLowerNetwork, std::map<int, double> directionInHigherNetworks)
{
  //could by some more complicated expression
  double assignedNodeChange = directionInHigherNetworks[mNode->getId()];

  std::vector<VectorFieldPoint*> vectorFieldPoints = currentVectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    VectorFieldPoint* currentPoint = (*itPoint);
    std::map<int, double> coordinate = currentPoint->getCoordinate();
    std::map<int, double> direction = currentPoint->getDirection();
    std::map<int, double> newDirection;
    for(std::map<int, double>::iterator itDir=direction.begin(); itDir != direction.end(); itDir++)
    {
      int key = itDir->first;
      double currentDirectionValue = itDir->second;
      double value = currentDirectionValue*directionInLowerNetwork[key]*assignedNodeChange;
      newDirection[key] = value;
    }
    targetVectorField->addPoint(coordinate, newDirection);
  }
}

std::map<int, double> VectorFieldReconfigurationImpl::calculateLowerNetworkDirection()
{
  Network* networkAssigned = mNode->getNetworkAssigned();
  std::map<int, double> currentState = networkAssigned->getCurrentState();
  return  networkAssigned->getDirectionAtState(currentState);
}

std::map<int, double> VectorFieldReconfigurationImpl::calculateHigherNetworksDirection()
{
  std::vector<std::map<int, double> > directionsInAllHigherNetworks;
  std::vector<Network*> networks = mNode->getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    std::map<int, double> currentState = (*itNet)->getCurrentState();
    directionsInAllHigherNetworks.push_back((*itNet)->getDirectionAtState(currentState));
  }
  std::map<int, double> sumDirectionInHigherNetworks;
  for(std::map<int, double>::iterator itDir=directionsInAllHigherNetworks[0].begin(); itDir != directionsInAllHigherNetworks[0].end(); ++itDir)
  {
    int key = itDir->first;
    double sumDir = 0;
    for(std::vector<std::map<int, double> >::iterator itNetD=directionsInAllHigherNetworks.begin(); itNetD != directionsInAllHigherNetworks.end(); ++itNetD)
    {
      sumDir += (*itNetD)[key];
    }
    sumDirectionInHigherNetworks[key] = sumDir;
  }
  return sumDirectionInHigherNetworks;
}
