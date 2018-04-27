#include <boost/test/unit_test.hpp>
#include "Network.hh"
#include "Node.hh"
#include "TestUtility.hh"

BOOST_AUTO_TEST_SUITE(TestSuiteStructureBuilding)

//------ID UNIQUENESS----------

BOOST_AUTO_TEST_CASE(addLayer_LayerIdUniqueness)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  for(int i=0; i<10; ++i)
  {
    multilayerNetwork->addLayer();
  }

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<int> layerIds;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    layerIds.push_back((*it)->getId());
  }

  bool isLayerIdDuplicated = containsDuplicate(layerIds);

  BOOST_CHECK_MESSAGE(isLayerIdDuplicated == false, "Duplicated layer id.");

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
