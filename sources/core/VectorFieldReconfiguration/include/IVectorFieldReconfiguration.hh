#ifndef IVECTORFIELDRECONFIGURATION_HH
#define IVECTORFIELDRECONFIGURATION_HH

#include "MultilayerNetwork.hh"
#include "GlobalVariables.hh"
#include "SimulationParameterContainer.hh"

class IVectorFieldReconfiguration
{
public:
  IVectorFieldReconfiguration(Node* node){mNode = node;};
  virtual ~IVectorFieldReconfiguration(void){};

  virtual void calculateVectorFieldReconfiguration(SimulationParameterContainer *parameters) = 0;
protected:
  Node* mNode;
};

#endif
