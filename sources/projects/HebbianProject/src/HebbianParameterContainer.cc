#include "HebbianParameterContainer.hh"

std::string HebbianParameterContainer::toString()
{
  std::string strParams;
  strParams.append("Hebbian Parameters");
  strParams.append("\n\n");
  
  strParams.append("higherNetworkSize = ");
  strParams.append(std::to_string(higherNetworkSize));
  strParams.append("\n");

  strParams.append("lowerNetworkSize = ");
  strParams.append(std::to_string(lowerNetworkSize));
  strParams.append("\n");

  strParams.append("transientTime = ");
  strParams.append(std::to_string(transientTime));
  strParams.append("\n");

  strParams.append("runTime = ");
  strParams.append(std::to_string(runTime));
  strParams.append("\n");

  strParams.append("higherNetworkInitConditionMin = ");
  strParams.append(std::to_string(higherNetworkInitConditionMin));
  strParams.append("\n");

  strParams.append("higherNetworkInitConditionMax = ");
  strParams.append(std::to_string(higherNetworkInitConditionMax));
  strParams.append("\n");

  strParams.append("lowerNetworkInitConditionMin = ");
  strParams.append(std::to_string(lowerNetworkInitConditionMin));
  strParams.append("\n");

  strParams.append("lowerNetworkInitConditionMax = ");
  strParams.append(std::to_string(lowerNetworkInitConditionMax));
  strParams.append("\n");

  return strParams;
}
