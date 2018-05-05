#ifndef VECTORFIELDRECONFIGURATIONIMPL_HH
#define VECTORFIELDRECONFIGURATIONIMPL_HH

#include "IVectorFieldReconfiguration.hh"
#include "GlobalVariables.hh"

class VectorField;

class VectorFieldReconfigurationImpl: public IVectorFieldReconfiguration
{
public:
  VectorFieldReconfigurationImpl(Node* node):IVectorFieldReconfiguration(node){mNode = node;};
  ~VectorFieldReconfigurationImpl(void){};

  void calculateVectorFieldReconfiguration(SimulationParameterContainer *parameters);
};

#endif
