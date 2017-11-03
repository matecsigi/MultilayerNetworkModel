#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "StructureGeneratorImpl.hh"

using namespace std;

int main(void)
{
  std::cout<<"Hello World!"<<std::endl;

  MultilayerNetwork multiNetwork;
  for(int i;i<3;i++)
  {
    multiNetwork.addLayer(i);
  }

  std::vector<Layer*> layers = multiNetwork.getLayers();
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
      }
  }
  
  cout<<multiNetwork;

  StructureGeneratorImpl generator;
  generator.generateStructure();

  return 0;
}
