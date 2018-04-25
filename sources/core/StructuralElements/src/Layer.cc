#include "Layer.hh"
#include "NetworkUtilityFunctions.hh"
#include "Trace.hh"
#include <iostream>
#include <map>

Layer::Layer()
{
  mMultilayerNetwork = NULL;
}

Layer::Layer(int id)
{
  mLayerId = id;
  mMultilayerNetwork = NULL;
}

Layer::~Layer(void)
{
  for(std::vector<Network*>::iterator it = mNetworks.begin(); it != mNetworks.end(); ++it)
  {
    delete (*it);
  }
}

int Layer::getTime()
{
  return mMultilayerNetwork->getTime();
}

void Layer::addNetwork(int networkId)
{
  Network *newNetwork = new Network(networkId);
  newNetwork->setMultilayerNetwork(mMultilayerNetwork);
  mNetworks.push_back(newNetwork);
}

Network* Layer::insertNetwork(Network* network)
{
  Network* newNetwork = new Network;
  copyNetwork(network, newNetwork);
  setIds(this, newNetwork);
  newNetwork->loadNodesToEquations();
  newNetwork->setMultilayerNetwork(mMultilayerNetwork);
  mNetworks.push_back(newNetwork);
  return newNetwork;
}

std::vector<Network*> Layer::getNetworks(void) const
{
  return mNetworks;
}

int Layer::getId(void) const
{
  return mLayerId;
}

void Layer::setMultilayerNetwork(MultilayerNetwork *multilayerNetwork)
{
  mMultilayerNetwork = multilayerNetwork;
  for(std::vector<Network*>::iterator itNet=mNetworks.begin(); itNet != mNetworks.end(); ++itNet)
  {
    (*itNet)->setMultilayerNetwork(multilayerNetwork);
  }
}

MultilayerNetwork* Layer::getMultilayerNetwork()
{
  return mMultilayerNetwork;
}

void Layer::print()
{
  traceDebug("Layer "+std::to_string(mLayerId)+"\n");
  for(std::vector<Network*>::iterator itNet=mNetworks.begin(); itNet != mNetworks.end(); ++itNet)
  {
    traceDebug("    ");
    (*itNet)->print();
  }
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
