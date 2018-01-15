#include "VectorFieldReconfigurationImpl.hh"
#include "VectorField.hh"
#include "VectorFieldSchemes.hh"
#include "VectorFieldTransformImplementations.hh"
#include "GeneticAlgorithmController.hh"

void VectorFieldReconfigurationImpl::calculateVectorFieldReconfiguration(std::vector<double> &modificationTypeProbabilities)
{
  mModificationTypeProbabilities = modificationTypeProbabilities;
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

  GeneticAlgorithmController geneticController(mModificationTypeProbabilities);
  geneticController.fitToVectorField(networkAssigned, targetVectorField);

  delete currentVectorField;
  delete targetVectorField;
}
