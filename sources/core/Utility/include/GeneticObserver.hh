#ifndef GENETICOBSERVER_HH
#define GENETICOBSERVER_HH

#include "IGeneticObserver.hh"

class GeneticObserver: public IGeneticObserver
{
public:
  GeneticObserver(GeneticAlgorithmController *geneticAlgorithmController):IGeneticObserver(geneticAlgorithmController){mGeneticAlgorithmController = geneticAlgorithmController;}
  ~GeneticObserver(void);
  
  virtual void atStart();
  virtual void atStep();
  virtual void atFinish();
};

#endif
