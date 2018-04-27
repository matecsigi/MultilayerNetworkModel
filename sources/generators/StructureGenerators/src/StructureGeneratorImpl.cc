#include "StructureGeneratorImpl.hh"
#include "BarabasiModel.hh"
#include <iostream>
#include <algorithm>
#include <math.h>

void StructureGeneratorImpl::generateStructure(int numberOfNodesPerNetwork, int numberOfLayers)
{
  for(int i=0; i<numberOfLayers; ++i)
  {
    mMultilayerNetwork->addLayer();
  }

  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  Network* networkUp = new Network;
  generateNetwork(networkUp, numberOfNodesPerNetwork);
  Network* insertedNetworkUp = layers[0]->insertNetwork(networkUp);

  std::vector<Node*> nodes = insertedNetworkUp->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    recursiveGenerator((*itNode), 2, numberOfNodesPerNetwork, numberOfLayers);
  }
}

void StructureGeneratorImpl::recursiveGenerator(Node *node, int depth, int numberOfNodesPerNetwork, int numberOfLayers)
{
  if(depth > numberOfLayers)
  {
    return;
  }
  else
  {
    std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
    Network* networkDown = new Network;
    generateNetwork(networkDown, numberOfNodesPerNetwork);
    Network* insertedNetworkDown = layers[depth-1]->insertNetwork(networkDown);
    node->setNetworkAssigned(insertedNetworkDown);
    std::vector<Node*> nodes = insertedNetworkDown->getNodes();
    for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
    {
      recursiveGenerator((*itNode), depth+1, numberOfNodesPerNetwork, numberOfLayers);
    }
    delete networkDown;
  }
}
