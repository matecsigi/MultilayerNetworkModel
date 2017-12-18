#include "VectorFieldReconfigurationImpl.hh"
#include "VectorField.hh"
#include "VectorFieldSchemes.hh"
#include "GeneticAlgorithmController.hh"

void VectorFieldReconfigurationImpl::calculateVectorFieldReconfiguration()
{
  // std::cout<<"VectorFieldReconfig node="<<mNode->getId()<<std::endl;
  Network* networkAssigned = mNode->getNetworkAssigned();
  if(networkAssigned == NULL)
  {
    return;
  }
  VectorField* currentVectorField = new VectorField();
  std::vector<IdValuePair> currentState = networkAssigned->getCurrentState();
  gridAroundPointScheme2(currentVectorField, networkAssigned, currentState);

  VectorField* targetVectorField = new VectorField();
  std::vector<IdValuePair> directionInLowerNetwork = calculateLowerNetworkDirection();
  std::vector<IdValuePair> directionInHigherNetworks = calculateHigherNetworksDirection();
  calculateTargetVectorField(targetVectorField, currentVectorField, directionInLowerNetwork, directionInHigherNetworks);

  // std::cout<<"--------Current---------"<<std::endl;;
  // std::cout<<*currentVectorField;
  // std::cout<<"--------Target----------"<<std::endl;
  // std::cout<<*targetVectorField;

  // Network* testNetwork = new Network;
  // NetworkModifier modifier;
  // modifier.copyNetwork(networkAssigned, testNetwork);

  // std::cout<<"DISTANCE="<<targetVectorField->getDistanceFrom(currentVectorField)<<std::endl;

  GeneticAlgorithmController geneticController;
  geneticController.fitToVectorField(networkAssigned, targetVectorField);

  // if(*testNetwork == *networkAssigned)
  // {
  //   std::cout<<"EQUAL"<<std::endl;
  // }
  // else
  // {
  //   std::cout<<"NOT EQUAL"<<std::endl;
  // }

  delete currentVectorField;
  delete targetVectorField;
}

void VectorFieldReconfigurationImpl::calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, std::vector<IdValuePair> directionInLowerNetwork, std::vector<IdValuePair> directionInHigherNetworks)
{
  //could by some more complicated expression
  double assignedNodeChange = getValueForId(directionInHigherNetworks, mNode->getId());

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
      // std::cout<<"Multiply="<<directionInLowerNetwork[key]<<" "<<assignedNodeChange<<std::endl;
      double value = currentDirectionValue*getValueForId(directionInLowerNetwork, key)*assignedNodeChange;
      setValueForId(newDirection, key, value);
    }
    targetVectorField->addPoint(coordinate, newDirection);
  }
}

std::vector<IdValuePair> VectorFieldReconfigurationImpl::calculateLowerNetworkDirection()
{
  Network* networkAssigned = mNode->getNetworkAssigned();
  std::vector<IdValuePair> currentState = networkAssigned->getCurrentState();
  return  networkAssigned->getDirectionAtState(currentState);
}

std::vector<IdValuePair> VectorFieldReconfigurationImpl::calculateHigherNetworksDirection()
{
  std::vector<std::vector<IdValuePair>> directionsInAllHigherNetworks;
  std::vector<Network*> networks = mNode->getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    std::vector<IdValuePair> currentState = (*itNet)->getCurrentState();
    directionsInAllHigherNetworks.push_back((*itNet)->getDirectionAtState(currentState));
  }
  std::vector<IdValuePair> sumDirectionInHigherNetworks;
  for(std::vector<IdValuePair>::iterator itDir=directionsInAllHigherNetworks[0].begin(); itDir != directionsInAllHigherNetworks[0].end(); ++itDir)
  {
    int key = itDir->mValue;
    double sumDir = 0;
    for(std::vector<std::vector<IdValuePair>>::iterator itNetD=directionsInAllHigherNetworks.begin(); itNetD != directionsInAllHigherNetworks.end(); ++itNetD)
    {
      sumDir += getValueForId((*itNetD), key);
    }
    setValueForId(sumDirectionInHigherNetworks, key, sumDir);
  }
  return sumDirectionInHigherNetworks;
}
