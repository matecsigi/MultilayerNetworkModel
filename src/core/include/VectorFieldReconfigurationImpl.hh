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
  void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, std::map<int, double> directionInLowerNetwork, std::map<int, double> directionInHigherNetworks);

  std::map<int, double> calculateLowerNetworkDirection();
  std::map<int, double> calculateHigherNetworksDirection();
};

#endif
