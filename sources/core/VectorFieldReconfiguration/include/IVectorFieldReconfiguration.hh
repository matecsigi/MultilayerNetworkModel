#ifndef IVECTORFIELDRECONFIGURATION_HH
#define IVECTORFIELDRECONFIGURATION_HH

#include "MultilayerNetwork.hh"
#include "GlobalVariables.hh"
#include "GeneticAlgorithmParameterContainer.hh"

class IVectorFieldReconfiguration
{
public:
  IVectorFieldReconfiguration(Node* node){mNode = node;};
  virtual ~IVectorFieldReconfiguration(void){};

  virtual void calculateVectorFieldReconfiguration(GeneticAlgorithmParameterContainer *geneticParameters) = 0;
protected:
  Node* mNode;
};

#endif
