#include "Layer.hh"
#include <iostream>

using namespace std;

Layer::Layer(void)
{
  cout<<"Creating layer"<<endl;
}

Layer::Layer(int id)
{
  mLayerId = id;
  cout<<"Creating layer with id="<<mLayerId<<endl;
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

std::vector<Network*> Layer::getNetworks(void)
{
  return mNetworksInLayer;
}
