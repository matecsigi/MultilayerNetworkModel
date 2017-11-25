#include "VectorFieldReconfigurationImpl.hh"
#include "VectorField.hh"
#include "VectorFieldSchemes.hh"

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

  delete currentVectorField;
  delete targetVectorField;
}
