#include "MultilayerNetworkClassUtility.hh"
#include "MultilayerNetwork.hh"
#include "Network.hh"
#include "Node.hh"
#include "LayerClassUtility.hh"
#include "NodeClassUtility.hh"
#include <algorithm>
#include <boost/mpi.hpp>

void shiftBuffers(MultilayerNetwork *multilayerNetwork)
{
  double* tmpBufferOld = new double[bufferSize];
  double* tmpBufferNew = new double[bufferSize];
  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    Layer* currentLayer = (*itLay);
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      Network* currentNetwork = (*itNet);
      std::vector<Node*> nodes = currentNetwork->getNodes();
      for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	Node* currentNode = (*itNode);
	currentNode->getValues(tmpBufferOld);
	for(int i=0; i<bufferSize; ++i)
	{
	  tmpBufferNew[i] = 0;
	}
	for(int i=0; i<initialConditionsSize; ++i)
	{
	  tmpBufferNew[i] = tmpBufferOld[(bufferSize-initialConditionsSize)+i];
	}
	currentNode->setValues(tmpBufferNew);
      }
    }
  }
  delete [] tmpBufferOld;
  delete [] tmpBufferNew;
}

double getMaxValue(MultilayerNetwork *multilayerNetwork)
{
  double max = 0;
  for(std::vector<int>::iterator itId=multilayerNetwork->mNodeIds.begin(); itId != multilayerNetwork->mNodeIds.end(); ++itId)
  {
    Node* node = multilayerNetwork->mNodesMap[*itId];
    if(getMaxValue(node) > max)
    {
      max = getMaxValue(node);
    }
  }
  return max;
}

double getMinValue(MultilayerNetwork *multilayerNetwork)
{
  double min = 0;
  for(std::vector<int>::iterator itId=multilayerNetwork->mNodeIds.begin(); itId != multilayerNetwork->mNodeIds.end(); ++itId)
  {
    Node* node = multilayerNetwork->mNodesMap[*itId];
    if(getMinValue(node) < min)
    {
      min = getMinValue(node);
    }
  }
  return min;
}

void updateNodesMap(MultilayerNetwork *multilayerNetwork)
{
  collectNodes(multilayerNetwork, multilayerNetwork->mNodesMap, multilayerNetwork->mNodeIds);

  /* This is needed so that each node is stepped only once,
   even if it's part of multiple networks*/
  std::sort(multilayerNetwork->mNodeIds.begin(), multilayerNetwork->mNodeIds.end());
  multilayerNetwork->mNodeIds.erase(unique(multilayerNetwork->mNodeIds.begin(), multilayerNetwork->mNodeIds.end()), multilayerNetwork->mNodeIds.end());
}

void collectNodes(MultilayerNetwork *multilayerNetwork, std::map<int, Node*>& nodesMap, std::vector<int>& nodeIds)
{
  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    Layer* currentLayer = (*itLay);
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      Network* currentNetwork = (*itNet);
      std::vector<Node*> nodes = currentNetwork->getNodes();
      for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	Node* currentNode = (*itNode);
	nodesMap[currentNode->getId()] = currentNode;
	nodeIds.push_back(currentNode->getId());
      }
    }
  }
}

void collectNetworks(MultilayerNetwork *multilayerNetwork, std::map<int, Network*>& networksMap, std::vector<int>& networkIds)
{
  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    Layer* currentLayer = (*itLay);
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      networksMap[(*itNet)->getId()] = (*itNet);
      networkIds.push_back((*itNet)->getId());
    }
  }
}

bool initialConditionsEqual(MultilayerNetwork& multilayerNetwork1, MultilayerNetwork& multilayerNetwork2)
{
  std::map<int, Node*> nodesMap1;
  std::vector<int> nodeIds1;
  collectNodes(&multilayerNetwork1, nodesMap1, nodeIds1);

  std::map<int, Node*> nodesMap2;
  std::vector<int> nodeIds2;
  collectNodes(&multilayerNetwork2, nodesMap2, nodeIds2);
  
  double* tmpBuffer1 = new double[bufferSize];
  double* tmpBuffer2 = new double[bufferSize];
  for(std::vector<int>::iterator itId=nodeIds1.begin(); itId != nodeIds1.end(); ++itId)
  {
    Node* node1 = nodesMap1[(*itId)];
    Node* node2 = nodesMap2[(*itId)];
    node1->getValues(tmpBuffer1);
    node2->getValues(tmpBuffer2);
    for(int i=0; i<bufferSize; ++i)
    {
      if(tmpBuffer1[i] != tmpBuffer2[i])
      {
	return false;
      }
    }
  }
  delete [] tmpBuffer1;
  delete [] tmpBuffer2;

  return true;
}

bool dynamicalEquationsEqual(MultilayerNetwork& multilayerNetwork1, MultilayerNetwork& multilayerNetwork2)
{
  std::map<int, Node*> nodesMap1;
  std::vector<int> nodeIds1;
  collectNodes(&multilayerNetwork1, nodesMap1, nodeIds1);

  std::map<int, Node*> nodesMap2;
  std::vector<int> nodeIds2;
  collectNodes(&multilayerNetwork2, nodesMap2, nodeIds2);

  std::map<int, Network*> networksMap1;
  std::vector<int> networkIds1;
  collectNetworks(&multilayerNetwork1, networksMap1, networkIds1);

  std::map<int, Network*> networksMap2;
  std::vector<int> networkIds2;
  collectNetworks(&multilayerNetwork2, networksMap2, networkIds2);
  
  for(std::vector<int>::iterator itId=nodeIds1.begin(); itId != nodeIds1.end(); ++itId)
  {
    Node* node1 = nodesMap1[(*itId)];
    Node* node2 = nodesMap2[(*itId)];
    std::vector<Network*> networks1 = node1->getNetworks();
    std::vector<Network*> networks2 = node2->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks1.begin(); itNet != networks1.end(); ++itNet)
    {
      Network* network1 = networksMap1[(*itNet)->getId()];
      Network* network2 = networksMap2[(*itNet)->getId()];
      std::string dynamicEquation1 = network1->getNodeDynamicalEquationString(node1->getId());
      std::string dynamicEquation2 = network2->getNodeDynamicalEquationString(node2->getId());
      if(dynamicEquation1 != dynamicEquation2)
      {
	return false;
      }
    }
  }
  
  return true;
}

bool multilayerNetworksEqual(MultilayerNetwork& multilayerNetwork1, MultilayerNetwork& multilayerNetwork2)
{
  std::vector<Layer*> layers1 = multilayerNetwork1.getLayers();
  std::vector<Layer*> layers2 = multilayerNetwork2.getLayers();
  if(layers1.size() != layers2.size())
  {
    return false;
  }
  std::map<int, Layer*> layerMap;
  for(std::vector<Layer*>::iterator itLay=layers1.begin(); itLay != layers1.end(); ++itLay)
  {
    layerMap[(*itLay)->getId()] = (*itLay);
  }

  for(std::vector<Layer*>::iterator itLay=layers2.begin(); itLay != layers2.end(); ++itLay)
  {
    if(layerMap.count((*itLay)->getId()))
    {
      Layer* currentLayer = (*itLay);
      Layer* correspondingLayer = layerMap[currentLayer->getId()];
      if(!(layersEqual(*currentLayer, *correspondingLayer)))
      {
	return false;
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}

void calculateClusterMessageSizes(MultilayerNetwork *multilayerNetwork, SimulationParameterContainer *parameters)
{
  int argc;
  char **argv = NULL;
  boost::mpi::environment env{argc, argv};
  boost::mpi::communicator world;

  parameters->geneticParameters->clusterMessageSizes.clear();

  for(int i=0; i< world.size(); ++i)
  {
    parameters->geneticParameters->clusterMessageSizes.push_back(0);
  }

  for(std::vector<int>::iterator itId=multilayerNetwork->mNodeIds.begin(); itId != multilayerNetwork->mNodeIds.end(); ++itId)
  {
    Node* node = multilayerNetwork->mNodesMap[(*itId)];
    if(node->getNetworkAssigned() != NULL)
    {
      int rank = (node->getId()%(world.size()-1))+1;
      parameters->geneticParameters->clusterMessageSizes[rank] += 1;
    }
  }
  // for(int i=0; i<world.size(); ++i)
  // {
  //   std::cout<<"rank="<<i<<" -> "<<parameters->geneticParameters->clusterMessageSizes[i]<<std::endl;
  // }
}
