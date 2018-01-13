#ifndef IGENETICOBSERVER_HH
#define IGENETICOBSERVER_HH

class GeneticAlgorithmController;

class IGeneticObserver
{
public:
  IGeneticObserver(GeneticAlgorithmController *geneticAlgorithmController){mGeneticAlgorithmController = geneticAlgorithmController;}
  ~IGeneticObserver(void){};

  virtual void atStart() = 0;
  virtual void atStep() = 0;
  virtual void atFinish() = 0;

protected:
  GeneticAlgorithmController *mGeneticAlgorithmController;
};

#endif
