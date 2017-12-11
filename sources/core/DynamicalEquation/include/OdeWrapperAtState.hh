#ifndef ODEWRAPPERATSTATE_HH
#define ODEWRAPPERATSTATE_HH

#include "DynamicalEquation.hh"

class OdeWrapperAtState
{
public:
  OdeWrapperAtState(DynamicalEquation* dynamicalEquation, std::map<int, double>& startingState)
  {
    mDynamicalEquation = dynamicalEquation;
    mStartingState = startingState;
  }
  ~OdeWrapperAtState(){};

  void operator() (const state_type &x, state_type &dxdt, double t)
  {
    mDynamicalEquation->ODEcalculatorAtState(x, dxdt, t, mStartingState);
  }
private:
  DynamicalEquation* mDynamicalEquation;
  std::map<int, double> mStartingState;
};

#endif
