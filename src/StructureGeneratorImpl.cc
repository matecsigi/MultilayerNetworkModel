#include "StructureGeneratorImpl.hh"
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

MultilayerNetwork StructureGeneratorImpl::generateStructure(void)
{
  MultilayerNetwork multilayerNetwork;
  for(int i=0;i<3;i++)
  {
    multilayerNetwork.addLayer(i);
  }

  std::vector<Layer*> layers = multilayerNetwork.getLayers();
  int networkIdCounter = 0;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    int layerIndex = std::distance(layers.begin(), it);
    for(int i=0; i<pow(3, (layerIndex+1)); ++i)
    {
      (*it)->addNetwork(networkIdCounter);
      ++networkIdCounter;
    }
  }

  int nodeIdCounter = 0;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    std::vector<Network*> networks = (*it)->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
      {
	for(int i=0; i<3; i++)
	{
	  (*it2)->addNode(nodeIdCounter);
	  ++nodeIdCounter;
	}
	(*it2)->generateConnections();
      }
  }
  
  //assign networks to nodes
  for(unsigned i=0; i<layers.size()-1; ++i)
  {
      std::vector<Network*> networksUp = layers[i]->getNetworks();
      std::vector<Network*> networksDown = layers[i+1]->getNetworks();
      int nodeCounter = 0;
      for(std::vector<Network*>::iterator it2=networksUp.begin(); it2 != networksUp.end(); ++it2)
      {
  	std::vector<Node*> nodesInNetwork = (*it2)->getNodes();
  	for(std::vector<Node*>::iterator it3=nodesInNetwork.begin(); it3 != nodesInNetwork.end(); ++it3)
  	{
  	  (*it3)->assignToNetwork(networksDown[nodeCounter]);
  	  ++nodeCounter;
  	}
      }
  }

  cout<<multilayerNetwork;

  return multilayerNetwork;
}
