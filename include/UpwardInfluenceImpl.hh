#ifndef UPWARDINFLUENCEIMPL_HH
#define UPWARDINFLUENCEIMPL_HH

#include "IUpwardInfluence.hh"

class UpwardInfluenceImpl: public IUpwardInfluence
{
public:
  UpwardInfluenceImpl(void){};
  ~UpwardInfluenceImpl(void){};

  double calculateUpwardInfluence();
};

#endif
