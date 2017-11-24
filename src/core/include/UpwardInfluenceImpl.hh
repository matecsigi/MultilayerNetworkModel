#ifndef UPWARDINFLUENCEIMPL_HH
#define UPWARDINFLUENCEIMPL_HH

#include "IUpwardInfluence.hh"

class UpwardInfluenceImpl: public IUpwardInfluence
{
public:
  UpwardInfluenceImpl(Node* node):IUpwardInfluence(node){mNode = node;};
  ~UpwardInfluenceImpl(void){};

  void calculateUpwardInfluence();
};

#endif
