#ifndef IDOWNWARDINFLUENCE_HH
#define IDOWNWARDINFLUENCE_HH

#include "MultilayerNetwork.hh"

class IDownwardInfluence
{
public:
  IDownwardInfluence(Node* node){mNode = node;};
  virtual ~IDownwardInfluence(void){};

  virtual void calculateDownwardInfluence(double downwardMultiplier) = 0;
protected:
  Node* mNode;
};

#endif
