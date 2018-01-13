#ifndef GENETICOBSERVER_HH
#define GENETICOBSERVER_HH

#include "IGeneticObserver.hh"

class GeneticObserver: public IGeneticObserver
{
public:
  GeneticObserver(GeneticAlgorithmController *geneticAlgorithmController):IGeneticObserver(geneticAlgorithmController){mGeneticAlgorithmController = geneticAlgorithmController;}
  ~GeneticObserver(void);
  
  void atStart();
  void atStep();
  void atFinish();
};

#endif
