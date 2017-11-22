#ifndef ODEWRAPPER_HH
#define ODEWRAPPER_HH

#include "DynamicalEquation.hh"

class OdeWrapper
{
public:
  OdeWrapper(DynamicalEquation* dynamicalEquation)
  {
    mDynamicalEquation = dynamicalEquation;
  }
  ~OdeWrapper(){};

  void operator() (const state_type &x, state_type &dxdt, double t)
  {
    mDynamicalEquation->ODEcalculator(x, dxdt, t);
  }
private:
  DynamicalEquation* mDynamicalEquation;
};

#endif
