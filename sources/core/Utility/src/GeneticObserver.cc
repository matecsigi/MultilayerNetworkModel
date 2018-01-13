#include "GeneticObserver.hh"
#include <iostream>

GeneticObserver::~GeneticObserver(void)
{

}

void GeneticObserver::atStart()
{
  std::cout<<"Genetic atStart"<<std::endl;
}

void GeneticObserver::atStep()
{
  std::cout<<"Genetic atStep"<<std::endl;
}

void GeneticObserver::atFinish()
{
  std::cout<<"Genetic atFinish"<<std::endl;
}
