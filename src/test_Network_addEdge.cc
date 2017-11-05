#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UnitTests
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <algorithm>
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"
#include "Node.hh"
#include "Network.hh"

int vectorHasDuplicatedElement(std::vector<int>& vectorToCheck)
{
  std::sort(vectorToCheck.begin(), vectorToCheck.end());
  for(unsigned i=0; i<vectorToCheck.size()-2; ++i)
  {
std::cout<<"i="<<i<<std::endl;
    if(vectorToCheck[i] == vectorToCheck[i+1])
    {
      return 1;
    }
  }
  return 0;
}

//testing if addEdge() creates duplicated edges if a connection is added twice
BOOST_AUTO_TEST_CASE(addEdge_duplicatedEdges)
{
  Network network;
  network.addNode(0);
  network.addNode(1);

  network.addEdge(0, 1);
  network.addEdge(0, 1);

  int numberOfEdges = network.getNodeNeighbors(0).size();

  BOOST_CHECK_MESSAGE(numberOfEdges == 1, "Duplicated edges.");
}

//testing the uniqueness of IDs for layers, networks and nodes
BOOST_AUTO_TEST_CASE(generateStructure_IdUniqueness)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl();
  generator->generateStructure(multilayerNetwork);

  std::cout<<"After generate"<<std::endl;

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<int> layerIds;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    layerIds.push_back((*it)->getId());
  }

std::cout<<"Got here"<<layerIds.size()<<std::endl;  

  int isLayerIdDuplicated = vectorHasDuplicatedElement(layerIds);

  std::cout<<"Here"<<std::endl;

  BOOST_CHECK_MESSAGE(isLayerIdDuplicated == 0, "Duplicated layer id.");

  delete multilayerNetwork;
}
