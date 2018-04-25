#include "StructureGeneratorImpl.hh"
#include "BarabasiModel.hh"
#include <iostream>
#include <algorithm>
#include <math.h>

void StructureGeneratorImpl::generateStructure()
{
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
