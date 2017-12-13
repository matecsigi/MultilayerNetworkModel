#ifndef VECTORFIELDRECONFIGURATIONIMPL_HH
#define VECTORFIELDRECONFIGURATIONIMPL_HH

#include "IVectorFieldReconfiguration.hh"
#include "VectorField.hh"

class VectorFieldReconfigurationImpl: public IVectorFieldReconfiguration
{
public:
  VectorFieldReconfigurationImpl(Node* node):IVectorFieldReconfiguration(node){mNode = node;};
  ~VectorFieldReconfigurationImpl(void){};

  void calculateVectorFieldReconfiguration();
  void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, std::vector<IdValuePair> directionInLowerNetwork, std::vector<IdValuePair> directionInHigherNetworks);

  std::vector<IdValuePair> calculateLowerNetworkDirection();
  std::vector<IdValuePair> calculateHigherNetworksDirection();
};

#endif
