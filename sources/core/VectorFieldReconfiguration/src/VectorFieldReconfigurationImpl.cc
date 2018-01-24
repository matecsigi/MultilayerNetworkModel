#include "VectorFieldReconfigurationImpl.hh"
#include "VectorField.hh"
#include "VectorFieldSchemes.hh"
#include "VectorFieldTransformImplementations.hh"
#include "GeneticAlgorithmController.hh"
#include "MpiUtility.hh"

void VectorFieldReconfigurationImpl::calculateVectorFieldReconfiguration(GeneticAlgorithmParameterContainer *geneticParameters)
{
  Network* networkAssigned = mNode->getNetworkAssigned();
  if(networkAssigned == NULL)
  {
    return;
  }
  VectorField* currentVectorField = new VectorField();
  std::vector<IdValuePair> currentState = networkAssigned->getCurrentState();
  gridAroundPointScheme2(currentVectorField, networkAssigned, currentState);

  VectorField* targetVectorField = new VectorField();
  // std::vector<IdValuePair> directionInLowerNetwork = calculateLowerNetworkDirection();
  // std::vector<IdValuePair> directionInHigherNetworks = calculateHigherNetworksDirection();
  weightNodeVectorFieldTransform(targetVectorField, currentVectorField, mNode);

  // std::cout<<"--------Current---------"<<std::endl;;
  // std::cout<<*currentVectorField;
  // std::cout<<"--------Target----------"<<std::endl;
  // std::cout<<*targetVectorField;

  if(geneticParameters->cluster == true)
  {
    mpiSend(1, 0, mNode->getId());
  }
  else
  {
    GeneticAlgorithmController geneticController(geneticParameters);
    geneticController.fitToVectorField(networkAssigned, targetVectorField);
  }

  delete currentVectorField;
  delete targetVectorField;
}
