#include "Layer.hh"
#include <iostream>

using namespace std;

Layer::Layer(void)
{
}

Layer::Layer(int id)
{
  mLayerId = id;
}

Layer::~Layer(void)
{
  std::cout<<"  Layer "<<this->getId()<<std::endl;  
  for(std::vector<Network*>::iterator it = mNetworksInLayer.begin(); it != mNetworksInLayer.end(); ++it)
  {
    delete (*it);
  }
}

void Layer::addNetwork(int networkId)
{
  mNetworksInLayer.push_back(new Network(networkId));
}

std::vector<Network*> Layer::getNetworks(void)
{
  return mNetworksInLayer;
}

int Layer::getId(void) const
{
  return mLayerId;
}
