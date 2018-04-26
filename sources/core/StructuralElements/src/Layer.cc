#include "Layer.hh"
#include "NetworkUtilityFunctions.hh"
#include "NetworkClassUtility.hh"
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
  loadNodesToEquations(newNetwork);
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
