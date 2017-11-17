#ifndef IUPWARDINFLUENCE_HH
#define IUPWARDINFLUENCE_HH

#include "MultilayerNetwork.hh"

class IUpwardInfluence
{
public:
  IUpwardInfluence(void){};
  virtual ~IUpwardInfluence(void){};

  virtual double calculateUpwardInfluence() = 0;
};

#endif
