#ifndef IVECTORFIELDRECONFIGURATION_HH
#define IVECTORFIELDRECONFIGURATION_HH

#include "MultilayerNetwork.hh"

class IVectorFieldReconfiguration
{
public:
  IVectorFieldReconfiguration(Node* node){mNode = node;};
  virtual ~IVectorFieldReconfiguration(void){};

  virtual void calculateVectorFieldReconfiguration() = 0;
protected:
  Node* mNode;
};

#endif
