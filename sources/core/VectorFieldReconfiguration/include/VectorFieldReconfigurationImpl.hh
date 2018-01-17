#ifndef VECTORFIELDRECONFIGURATIONIMPL_HH
#define VECTORFIELDRECONFIGURATIONIMPL_HH

#include "IVectorFieldReconfiguration.hh"
#include "VectorField.hh"
#include "GlobalVariables.hh"

class VectorFieldReconfigurationImpl: public IVectorFieldReconfiguration
{
public:
  VectorFieldReconfigurationImpl(Node* node):IVectorFieldReconfiguration(node){mNode = node;};
  ~VectorFieldReconfigurationImpl(void){};

  void calculateVectorFieldReconfiguration(GeneticAlgorithmParameterContainer *geneticParameters);
};

#endif
