#ifndef UPWARDINFLUENCEIMPL_HH
#define UPWARDINFLUENCEIMPL_HH

#include "IUpwardInfluence.hh"

class MultilayerNetwork;

class UpwardInfluenceImpl: public IUpwardInfluence
{
public:
  UpwardInfluenceImpl(Node* node):IUpwardInfluence(node){mNode = node;};
  ~UpwardInfluenceImpl(void){};

  void calculateUpwardInfluence(double upwardMultiplier);
};

#endif
