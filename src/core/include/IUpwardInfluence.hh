#ifndef IUPWARDINFLUENCE_HH
#define IUPWARDINFLUENCE_HH

#include "MultilayerNetwork.hh"

class IUpwardInfluence
{
public:
  IUpwardInfluence(Node* node){mNode = node;};
  virtual ~IUpwardInfluence(void){};

  virtual void calculateUpwardInfluence() = 0;
protected:
  Node* mNode;
};

#endif
