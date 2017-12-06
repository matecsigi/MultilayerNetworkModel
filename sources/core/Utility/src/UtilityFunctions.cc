#include "UtilityFunctions.hh"
#include "GlobalVariables.hh"

int getIndexT(int t)
{
  return (t%(bufferSize-2))+2;
}

int getIndexTMinusOne(int t)
{
  return (t%(bufferSize-2))+1;
}

int getIndexTMinusTwo(int t)
{
  return t%(bufferSize-2);
}

void loadNetworkFromJSON(Network* network, std::string filename)
{
  
}
