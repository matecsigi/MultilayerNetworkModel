#ifndef IVECTORFIELDRECONFIGURATION_HH
#define IVECTORFIELDRECONFIGURATION_HH

#include "GlobalVariables.hh"
#include "SimulationParameterContainer.hh"

class MultilayerNetwork;
class Node;

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
