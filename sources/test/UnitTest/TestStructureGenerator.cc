#include <boost/test/unit_test.hpp>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"

BOOST_AUTO_TEST_SUITE(TestSuiteStructureGenerator)

BOOST_AUTO_TEST_CASE(NetworkToNodeAssignment)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure(4, 2);

  bool noNetworkAssigned = false;

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != --layers.end(); ++itLay)
  {
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      std::vector<Node*> nodes = (*itNet)->getNodes();
      for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	if((*itNode)->getNetworkAssigned() == NULL)
	{
	  noNetworkAssigned = true;
	}
      }
    }
  }

  BOOST_CHECK_MESSAGE(noNetworkAssigned == false, "No network assigned to certain nodes");

  delete multilayerNetwork;
}

//testing if the networks assigned to nodes are all one layer lower than the nodes they are assigned to
BOOST_AUTO_TEST_CASE(AssignedNodeIsOneLayerHigherThanTheNetwork)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork();
  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure(5, 4);

  bool assignedToProperLayer = true;

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != --layers.end(); ++itLay)
  {
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      std::vector<Node*> nodes = (*itNet)->getNodes();
      for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	Network* networkAssigned = (*itNode)->getNetworkAssigned();
	Layer* layerOfAssignedNetwork = NULL;
	for(std::vector<Layer*>::iterator itLay2=layers.begin(); itLay2 != layers.end(); ++itLay2)
	{
	  std::vector<Network*> networksInLayer = (*itLay2)->getNetworks();
	  if(std::find(networksInLayer.begin(), networksInLayer.end(), networkAssigned) != networksInLayer.end())
	  {
	    layerOfAssignedNetwork = (*itLay2);
	  }
	}
	std::vector<Layer*>::iterator itTmp = itLay;
	++itTmp;
	if(layerOfAssignedNetwork != (*itTmp))
	{
	  assignedToProperLayer = false;
	}
      }
    }
  }

  BOOST_CHECK_MESSAGE(assignedToProperLayer == true, "The nodes assigned to networks are not one layer above");

  delete multilayerNetwork;
}


BOOST_AUTO_TEST_SUITE_END()
