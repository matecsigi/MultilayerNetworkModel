#include "MultilayerNetwork.hh"
#include <iostream>

using namespace std;

MultilayerNetwork::MultilayerNetwork(void)
{
  cout<<"Creating network"<<endl;
}

MultilayerNetwork::~MultilayerNetwork(void)
{
  for(std::vector<Layer*>::iterator it=mLayers.begin(); it != mLayers.end(); ++it)
  {
    delete (*it);
  }
}

void MultilayerNetwork::addLayer(int layerId)
{
  mLayers.push_back(new Layer(layerId));
}

std::vector<Layer*> MultilayerNetwork::getLayers(void)
{
  return mLayers;
}
