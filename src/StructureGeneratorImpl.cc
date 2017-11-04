#include "StructureGeneratorImpl.hh"
#include <iostream>

using namespace std;

MultilayerNetwork StructureGeneratorImpl::generateStructure(void)
{
  MultilayerNetwork multilayerNetwork;
  for(int i;i<3;i++)
  {
    multilayerNetwork.addLayer(i);
  }

  std::vector<Layer*> layers = multilayerNetwork.getLayers();
  int networkIdCounter = 0;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    for(int i=0; i<3; i++)
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
  
  cout<<multilayerNetwork;

  return multilayerNetwork;
}
