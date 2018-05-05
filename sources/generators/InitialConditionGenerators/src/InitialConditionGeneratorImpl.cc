#include "InitialConditionGeneratorImpl.hh"
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "NetworkInitialConditionGenerators.hh"

void InitialConditionGeneratorImpl::generateInitialCondition()
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    Layer* currentLayer = (*itLay);
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      Network* currentNetwork = (*itNet);
      oneNetworkInitialConditions(currentNetwork);
    }
  }
}
