#ifndef HEBBIANPARAMETERCONTAINER_HH
#define HEBBIANPARAMETERCONTAINER_HH

#include <string>

class HebbianParameterContainer
{
public:
  HebbianParameterContainer(){};
  ~HebbianParameterContainer(){};

  bool cluster = false;

  int higherNetworkSize = 20;
  int lowerNetworkSize = 10;

  int transientTime = 10;
  int runTime = 20;
  double higherNetworkInitConditionMin = 100;
  double higherNetworkInitConditionMax = 200;
  double lowerNetworkInitConditionMin = 0;
  double lowerNetworkInitConditionMax = 10;

  std::string toString();
};

#endif
