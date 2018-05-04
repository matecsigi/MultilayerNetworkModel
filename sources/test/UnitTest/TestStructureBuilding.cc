#include <boost/test/unit_test.hpp>
#include "Network.hh"
#include "Node.hh"
#include "TestUtility.hh"
#include "BarabasiModel.hh"
#include "UtilityFunctions.hh"

BOOST_AUTO_TEST_SUITE(TestSuiteStructureBuilding)

//------ID UNIQUENESS----------

BOOST_AUTO_TEST_CASE(IdUniqueness)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  Layer *layer1 = multilayerNetwork->addLayer();
  Layer *layer2 = multilayerNetwork->addLayer();
  Layer *layer3 = multilayerNetwork->addLayer();

  for(int i=0; i<5; ++i)
  {
    layer1->addNetwork();
    layer2->addNetwork();
    layer3->addNetwork();
  }

  Network* network1 = layer1->addNetwork();
  Network* network2 = layer2->addNetwork();

  for(int i=0; i<10; ++i)
  {
    network1->addNode();
    network2->addNode();
  }

  Network *network3 = new Network; 
  generateNetwork(network3, 10);
  layer1->insertNetwork(network3);
  Network *network4 = layer1->insertNetwork(network3);
  Network *network5 = layer3->insertNetwork(network3);

  for(int i=0; i<5; ++i)
  {
    network1->addNode();
    network4->addNode();
    network5->addNode();
  }

  std::vector<int> layerIds;
  std::vector<int> networkIds;
  std::vector<int> nodeIds;
  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    layerIds.push_back((*itLay)->getId());
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      networkIds.push_back((*itNet)->getId());
      std::vector<Node*> nodes=(*itNet)->getNodes();
      for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	nodeIds.push_back((*itNode)->getId());
      }
    }
  }

  bool isLayerIdDuplicated = containsDuplicate(layerIds);
  bool isNetworkIdDuplicated = containsDuplicate(networkIds);
  bool isNodeIdDuplicated = containsDuplicate(nodeIds);

  BOOST_CHECK_MESSAGE(isLayerIdDuplicated == false, "Duplicated layer id.");
  BOOST_CHECK_MESSAGE(isNetworkIdDuplicated == false, "Duplicated network id.");
  BOOST_CHECK_MESSAGE(isNodeIdDuplicated == false, "Duplicated node id.");

  delete network3;
  delete multilayerNetwork;
}

//-----------------------------

//testing if addEdge() creates duplicated edges if a connection is added twice
BOOST_AUTO_TEST_CASE(addEdge_DuplicatedEdges)
{
  Network *network = new Network;
  Node *node1 = network->addNode();
  Node *node2 = network->addNode();

  network->addEdge(node1->getId(), node2->getId());
  network->addEdge(node1->getId(), node2->getId());

  int numberOfEdges = network->getNodeNeighbors(node2->getId()).size();

  BOOST_CHECK_MESSAGE(numberOfEdges == 1, "Duplicated edges.");
  
  delete network;
}

BOOST_AUTO_TEST_SUITE_END()
