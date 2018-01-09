#ifndef VECTORFIELDRECONFIGURATIONIMPL_HH
#define VECTORFIELDRECONFIGURATIONIMPL_HH

#include "IVectorFieldReconfiguration.hh"
#include "VectorField.hh"
#include "GlobalVariables.hh"

class VectorFieldReconfigurationImpl: public IVectorFieldReconfiguration
{
public:
  VectorFieldReconfigurationImpl(Node* node, std::vector<double> &modificationTypeProbabilities = vectorReconfModTypeProbabilities):IVectorFieldReconfiguration(node, modificationTypeProbabilities){mNode = node; mModificationTypeProbabilities = modificationTypeProbabilities;};
  ~VectorFieldReconfigurationImpl(void){};

  void calculateVectorFieldReconfiguration();
  void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, std::vector<IdValuePair> directionInLowerNetwork, std::vector<IdValuePair> directionInHigherNetworks);

  std::vector<IdValuePair> calculateLowerNetworkDirection();
  std::vector<IdValuePair> calculateHigherNetworksDirection();
};

#endif
