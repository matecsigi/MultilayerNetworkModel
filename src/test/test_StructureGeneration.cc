#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UnitTests
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <algorithm>
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"
#include "IDynamicalEquationGenerator.hh"
#include "DynamicalEquationGeneratorSimpleImpl.hh"
#include "IInitialConditionGenerator.hh"
#include "InitialConditionGeneratorImpl.hh"
#include "Node.hh"
#include "Network.hh"

int vectorHasDuplicatedElement(std::vector<int>& vectorToCheck)
{
  std::sort(vectorToCheck.begin(), vectorToCheck.end());
  for(unsigned i=0; i<vectorToCheck.size()-1; ++i)
  {
    if(vectorToCheck[i] == vectorToCheck[i+1])
    {
      return 1;
    }
  }
  return 0;
}

BOOST_AUTO_TEST_SUITE(test_suite_StructureCorrectness)

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

//testing the uniqueness of IDs for layers
BOOST_AUTO_TEST_CASE(generateStructure_LayerIdUniqueness)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<int> layerIds;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    layerIds.push_back((*it)->getId());
  }

  int isLayerIdDuplicated = vectorHasDuplicatedElement(layerIds);

  BOOST_CHECK_MESSAGE(isLayerIdDuplicated == 0, "Duplicated layer id.");

  delete multilayerNetwork;
}

//testing the uniqueness of IDs for networks
BOOST_AUTO_TEST_CASE(generateStructure_NetworkIdUniqueness)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<int> networkIds;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    std::vector<Network*> networks = (*it)->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      networkIds.push_back((*it2)->getId()); 
    }
  }

  int isNetworkIdDuplicated = vectorHasDuplicatedElement(networkIds);

  BOOST_CHECK_MESSAGE(isNetworkIdDuplicated == 0, "Duplicated network id.");

  delete multilayerNetwork;
}

//testing the uniqueness of IDs for nodes
BOOST_AUTO_TEST_CASE(generateStructure_NodeIdUniqueness)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<int> nodeIds;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    std::vector<Network*> networks = (*it)->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      std::vector<Node*> nodes = (*it2)->getNodes();
      for(std::vector<Node*>::iterator it3=nodes.begin(); it3 != nodes.end(); ++it3)
      {
	nodeIds.push_back((*it3)->getId()); 
      }
    }
  }

  int isNodeIdDuplicated = vectorHasDuplicatedElement(nodeIds);

  BOOST_CHECK_MESSAGE(isNodeIdDuplicated == 0, "Duplicated node id.");

  delete multilayerNetwork;
}

//testing if every node except for the ones in the bottom layer
//are assigned a network
BOOST_AUTO_TEST_CASE(generateStructure_NetworkToNodeAssignment)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  int noNetworkAssigned = 0;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != --layers.end(); ++it)
  {
    std::vector<Network*> networks = (*it)->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      std::vector<Node*> nodes = (*it2)->getNodes();
      for(std::vector<Node*>::iterator it3=nodes.begin(); it3 != nodes.end(); ++it3)
      {
	if((*it3)->getNetworkAssigned() == NULL)
	{
	  noNetworkAssigned = 1;
	}
      }
    }
  }

  BOOST_CHECK_MESSAGE(noNetworkAssigned == 0, "No network assigned to certain nodes");

  delete multilayerNetwork;
}

//testing if every network except for the ones at the top layer
//are assigned a node in the higher layer
BOOST_AUTO_TEST_CASE(generateStructure_NodeToNetworkAssignment)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  int noNodeAssigned = 0;
  for(std::vector<Layer*>::iterator it=++layers.begin(); it != layers.end(); ++it)
  {
    std::vector<Network*> networks = (*it)->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      if((*it2)->getNodeAssigned() == NULL)
      {
	noNodeAssigned = 1;
      }
    }
  }

  BOOST_CHECK_MESSAGE(noNodeAssigned == 0, "No node assigned to certain networks");

  delete multilayerNetwork;
}

//testing if the networks assigned to nodes are all one layer lower than the nodes
//they are assigned to
BOOST_AUTO_TEST_CASE(generateStructure_AssignedNodeIsOneLayerHigherThanTheNetwork)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  int assignedToProperLayer = 1;
  for(std::vector<Layer*>::iterator it=layers.begin(); it != --layers.end(); ++it)
  {
    std::vector<Network*> networks = (*it)->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      std::vector<Node*> nodes = (*it2)->getNodes();
      for(std::vector<Node*>::iterator it3=nodes.begin(); it3 != nodes.end(); ++it3)
      {
	Network* networkAssigned = (*it3)->getNetworkAssigned();
	Layer* layerOfAssignedNetwork = NULL;
	for(std::vector<Layer*>::iterator it4=layers.begin(); it4 != layers.end(); ++it4)
	{
	  std::vector<Network*> networksInLayer = (*it4)->getNetworks();
	  if(std::find(networksInLayer.begin(), networksInLayer.end(), networkAssigned) != networksInLayer.end())
	  {
	    layerOfAssignedNetwork = (*it4);
	  }
	}
	std::vector<Layer*>::iterator itTmp = it;
	++itTmp;
	if(layerOfAssignedNetwork != (*itTmp))
	{
	  assignedToProperLayer = 0;
	}
      }
    }
  }

  BOOST_CHECK_MESSAGE(assignedToProperLayer == 1, "The nodes assigned to networks are not one layer above");

  delete multilayerNetwork;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_suite_SaveLoadFromJSON)

BOOST_AUTO_TEST_CASE(generateStructure_SavedStructureEqualsLoaded)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  const char *filename="generated/MultilayerNetworkStructure-1.json";
  multilayerNetwork->save(filename);

  MultilayerNetwork* multilayerNetwork2 = new MultilayerNetwork();
  multilayerNetwork2->load(filename);

  bool structuresEqual = (*multilayerNetwork == *multilayerNetwork2);

  BOOST_CHECK_MESSAGE(structuresEqual == true, "Saved and loaded structures don't match");

  delete multilayerNetwork;
  delete multilayerNetwork2;
}

BOOST_AUTO_TEST_CASE(generateStructure_SavedAndLoadedInitialConditionsMatch)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure();
  const char *filename="generated/MultilayerNetworkStructure-1.json";
  multilayerNetwork->save(filename);

  const char *filenameInitialCond="generated/InitialCondition-1.bin";
  IInitialConditionGenerator* initialConditionGenerator = new InitialConditionGeneratorImpl(multilayerNetwork);
  initialConditionGenerator->generateInitialCondition();
  multilayerNetwork->saveState(filenameInitialCond);

  MultilayerNetwork* multilayerNetwork2 = new MultilayerNetwork;
  multilayerNetwork2->load(filename);
  multilayerNetwork2->loadState(filenameInitialCond);

  bool initialConditionsMatch = initialConditionsEqual(*multilayerNetwork, *multilayerNetwork2);

  BOOST_CHECK_MESSAGE(initialConditionsMatch == true, "Saved and loaded initial conditions don't match");

  delete multilayerNetwork;
  delete multilayerNetwork2;
}


BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(test_suite_DynamicalEquationTests)

BOOST_AUTO_TEST_CASE(generateStructure_SavedAndLoadedDynamicalEquationMatch)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure();
  const char *filename="generated/MultilayerNetworkStructure-1.json";

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  multilayerNetwork->save(filename);

  MultilayerNetwork* multilayerNetwork2 = new MultilayerNetwork;
  multilayerNetwork2->load(filename);

  bool dynamicalEquationsMatch = dynamicalEquationsEqual(*multilayerNetwork, *multilayerNetwork2);

  BOOST_CHECK_MESSAGE(dynamicalEquationsMatch == true, "Saved and loaded dynamical equations don't match");

  delete multilayerNetwork;
  delete multilayerNetwork2;

}

BOOST_AUTO_TEST_CASE(generateStructure_CalculationNodeIds)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure();

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  bool correctNodesAssigned = true;
  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      std::vector<Node*> nodes = (*itNet)->getNodes();
      for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	DynamicalEquation* dynamicalEquation = (*itNet)->getNodeDynamicalEquation((*itNode)->getId());
	if(dynamicalEquation->testNodeIds() == false)
	{
	  correctNodesAssigned = false;
	}
      }
    }
  }

  BOOST_CHECK_MESSAGE(correctNodesAssigned == true, "The IDs in CalculationNodes don't match with the node assigned to them");

  delete multilayerNetwork;
}

BOOST_AUTO_TEST_SUITE_END()
