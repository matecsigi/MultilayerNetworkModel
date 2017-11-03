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

std::vector<Layer*> MultilayerNetwork::getLayers(void) const
{
  return mLayers;
}

ostream& operator<<(ostream& os, const MultilayerNetwork& multilayerNetwork)
{
  os<<"<< operator"<<endl;
  std::vector<Layer*> layers = multilayerNetwork.getLayers();
  for(std::vector<Layer*>::iterator it1=layers.begin(); it1 != layers.end(); ++it1)
  {
    Layer* currentLayer = (*it1);
    os<<"-Layer "<<currentLayer->getId()<<endl;
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      Network* currentNetwork = (*it2);
      os<<"  --Network "<<currentNetwork->getId()<<endl;
      std::vector<Node*> nodes = currentNetwork->getNodes();
      for(std::vector<Node*>::iterator it3 = nodes.begin(); it3 != nodes.end(); ++it3)
      {
	Node* currentNode = (*it3);
	os<<"    ---Node "<<currentNode->getId()<<endl;
      }
    }
  }
  return os;
}

