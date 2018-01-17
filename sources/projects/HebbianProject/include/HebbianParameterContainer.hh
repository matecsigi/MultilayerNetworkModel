#ifndef HEBBIANPARAMETERCONTAINER_HH
#define HEBBIANPARAMETERCONTAINER_HH

class HebbianParameterContainer
{
public:
  HebbianParameterContainer(){};
  ~HebbianParameterContainer(){};

  int higherNetworkSize = 5;
  int lowerNetworkSize = 5;

  int transientTime = 0;
  int runTime = 20;

  double higherNetworkInitConditionMin = 10000;
  double higherNetworkInitConditionMax = 20000;
  double lowerNetworkInitConditionMin = 0;
  double lowerNetworkInitConditionMax = 10;
};

#endif
