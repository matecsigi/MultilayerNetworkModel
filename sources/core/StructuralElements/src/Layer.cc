#include "Layer.hh"
#include <iostream>
#include <map>

Layer::Layer(void)
{
}

Layer::Layer(int id)
{
  mLayerId = id;
}

Layer::~Layer(void)
{
  for(std::vector<Network*>::iterator it = mNetworksInLayer.begin(); it != mNetworksInLayer.end(); ++it)
  {
    delete (*it);
  }
}

void Layer::addNetwork(int networkId)
{
  mNetworksInLayer.push_back(new Network(networkId));
}

std::vector<Network*> Layer::getNetworks(void) const
{
  return mNetworksInLayer;
}

int Layer::getId(void) const
{
  return mLayerId;
}

bool operator==(const Layer& layer1, const Layer& layer2)
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
      if(!(*currentNetwork == *correspondingNetwork))
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
