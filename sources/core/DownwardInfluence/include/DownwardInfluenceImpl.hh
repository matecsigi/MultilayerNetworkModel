#ifndef DOWNWARDINFLUENCEIMPL_HH
#define DOWNWARDINFLUENCEIMPL_HH

#include "IDownwardInfluence.hh"

class MultilayerNetwork;

class DownwardInfluenceImpl: public IDownwardInfluence
{
public:
  DownwardInfluenceImpl(Node* node):IDownwardInfluence(node){mNode = node;};
  ~DownwardInfluenceImpl(void){};

  void calculateDownwardInfluence(double downwardMultiplier = 1);
};

#endif
