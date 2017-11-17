#ifndef IDOWNWARDINFLUENCE_HH
#define IDOWNWARDINFLUENCE_HH

#include "MultilayerNetwork.hh"

class IDownwardInfluence
{
public:
  IDownwardInfluence(void){};
  virtual ~IDownwardInfluence(void){};

  virtual double calculateDownwardInfluence() = 0;
};

#endif
