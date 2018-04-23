#include "GeneralNetworkGenerator.hh"

void generateNetworkGeneral(Network* network, int control, std::function<void(Network*)> structureGenerator, std::function<void(Network*)> dynamicsGenerator, std::function<void(Network*)> initialConditionGenerator)
{
  if(structureGenerator != nullptr)
  {
    structureGenerator(network);
  }
  if(dynamicsGenerator != nullptr)
  {
    dynamicsGenerator(network);
  }
  if(initialConditionGenerator != nullptr)
  {
    initialConditionGenerator(network);
  }
}
