#ifndef DOWNWARDINFLUENCEIMPL_HH
#define DOWNWARDINFLUENCEIMPL_HH

#include "IDownwardInfluence.hh"

class DownwardInfluenceImpl: public IDownwardInfluence
{
public:
  DownwardInfluenceImpl(void){};
  ~DownwardInfluenceImpl(void){};

  double calculateDownwardInfluence();
};

#endif
