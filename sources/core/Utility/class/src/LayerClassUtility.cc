#include "LayerClassUtility.hh"
#include "Layer.hh"
#include "NetworkClassUtility.hh"

bool layersEqual(const Layer& layer1, const Layer& layer2)
{
  std::vector<Network*> networks1 = layer1.getNetworks();
  std::vector<Network*> networks2 = layer2.getNetworks();
  if(networks1.size() != networks2.size())
  {
    return false;
  }
  std::map<int, Network*> networkMap;
  for(std::vector<Network*>::iterator itNet=networks1.begin(); itNet != networks1.end(); ++itNet)
  {
    networkMap[(*itNet)->getId()] = (*itNet);
  }

  for(std::vector<Network*>::iterator itNet=networks2.begin(); itNet != networks2.end(); ++itNet)
  {
    if(networkMap.count((*itNet)->getId()))
    {
      Network* currentNetwork = (*itNet);
      Network* correspondingNetwork = networkMap[currentNetwork->getId()];
      if(!(networksEqual(*currentNetwork, *correspondingNetwork)))
      {
	return false;
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}
