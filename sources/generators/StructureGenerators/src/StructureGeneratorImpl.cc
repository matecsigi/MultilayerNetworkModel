#include "StructureGeneratorImpl.hh"
#include "BarabasiModel.hh"
#include <iostream>
#include <algorithm>
#include <math.h>

void StructureGeneratorImpl::generateStructure()
{
  // generateLayers();
  // generateNetworks();
  // generateNodes();
  // assignNetworksToNodes();

  int numberOfNodes = 5;

  Layer* layer1 = mMultilayerNetwork->addLayer();
  Layer* layer2 = mMultilayerNetwork->addLayer();

  Network* networkUp = new Network;
  generateNetwork(networkUp, numberOfNodes);
  Network* insertedNetworkUp = layer1->insertNetwork(networkUp);

  std::vector<Node*> nodes = insertedNetworkUp->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Network *networkDown = new Network;
    generateNetwork(networkDown, numberOfNodes);
    Network* insertedNetworkDown = layer2->insertNetwork(networkDown);
    (*itNode)->setNetworkAssigned(insertedNetworkDown);
  }
}

void StructureGeneratorImpl::generateLayers(void)
{
  for(int i=1;i<4;i++)
  {
    mMultilayerNetwork->addLayerById(i);
  }
}

void StructureGeneratorImpl::generateNetworks(void)
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  int networkIdCounter = 0;
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    int layerIndex = std::distance(layers.begin(), itLay);
    for(int i=0; i<pow(3, (layerIndex+1)); ++i)
    {
      (*itLay)->addNetwork(networkIdCounter+1);
      ++networkIdCounter;
    }
  }
}

void StructureGeneratorImpl::generateNodes(void)
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  int nodeIdCounter = 0;
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
      {
	for(int i=0; i<3; i++)
	{
	  (*itNet)->addNodeById(nodeIdCounter+1);
	  ++nodeIdCounter;
	}
	generateEdges(*itNet);
      }
  }
}

void StructureGeneratorImpl::generateEdges(Network* network)
{
  //creating a circular graph
  std::vector<Node*> nodes = network->getNodes();
  for(unsigned i=1; i<nodes.size(); ++i)
  {
    int nodeId1 = nodes[i-1]->getId();
    int nodeId2 = nodes[i]->getId();
    network->addEdge(nodeId1, nodeId2);
  }
  if(nodes.size() > 1)
  {
    int nodeId1 = nodes[nodes.size()-1]->getId();
    int nodeId2 = nodes[0]->getId();
    network->addEdge(nodeId1, nodeId2);
  }
}

void StructureGeneratorImpl::assignNetworksToNodes(void)
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  for(unsigned i=0; i<layers.size()-1; ++i)
  {
    std::vector<Network*> networksUp = layers[i]->getNetworks();
    std::vector<Network*> networksDown = layers[i+1]->getNetworks();
    int nodeCounter = 0;
    for(std::vector<Network*>::iterator itNet=networksUp.begin(); itNet != networksUp.end(); ++itNet)
    {
      std::vector<Node*> nodesInNetwork = (*itNet)->getNodes();
      for(std::vector<Node*>::iterator itNode=nodesInNetwork.begin(); itNode != nodesInNetwork.end(); ++itNode)
      {
	(*itNode)->setNetworkAssigned(networksDown[nodeCounter]);
  	 ++nodeCounter;
      }
    }
  }
}
