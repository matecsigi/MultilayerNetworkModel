#ifndef HEBBIANOBSERVER_HH
#define HEBBIANOBSERVER_HH

#include "IObserver.hh"

class HebbianObserver: public IObserver
{
public:
  HebbianObserver(MultilayerNetwork* multilayerNetwork):IObserver(multilayerNetwork){mMultilayerNetwork = multilayerNetwork;};
  ~HebbianObserver(void){};

  void atStep();
  void atFinish();
};

#endif
