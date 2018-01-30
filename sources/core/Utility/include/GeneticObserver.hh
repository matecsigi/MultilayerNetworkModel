#ifndef GENETICOBSERVER_HH
#define GENETICOBSERVER_HH

#include "IGeneticObserver.hh"
// #include "spdlog/spdlog.h"

class GeneticObserver: public IGeneticObserver
{
public:
  GeneticObserver(GeneticAlgorithmController *geneticAlgorithmController):IGeneticObserver(geneticAlgorithmController){mGeneticAlgorithmController = geneticAlgorithmController;}
  ~GeneticObserver(void);
  
  void atStart();
  void atStep();
  void atFinish();
// private:
//   std::shared_ptr<spdlog::logger> mLogger;
};

#endif
