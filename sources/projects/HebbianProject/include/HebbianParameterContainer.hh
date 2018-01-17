#ifndef HEBBIANPARAMETERCONTAINER_HH
#define HEBBIANPARAMETERCONTAINER_HH

class HebbianParameterContainer
{
public:
  HebbianParameterContainer(){};
  ~HebbianParameterContainer(){};

  int higherNetworkSize = 10;
  int lowerNetworkSize = 10;

  int transientTime = 0;
  int runTime = 20;

  double higherNetworkInitConditionMin = 100;
  double higherNetworkInitConditionMax = 200;
  double lowerNetworkInitConditionMin = 0;
  double lowerNetworkInitConditionMax = 10;
};

#endif
