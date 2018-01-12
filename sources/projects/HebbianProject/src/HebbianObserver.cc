#include "HebbianObserver.hh"
#include <iostream>

void HebbianObserver::atStep()
{
  std::cout<<"atStep"<<std::endl;
}

void HebbianObserver::atFinish()
{
  std::cout<<"atFinish"<<std::endl;
}
