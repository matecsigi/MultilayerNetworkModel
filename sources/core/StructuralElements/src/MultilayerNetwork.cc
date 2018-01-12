#include "MultilayerNetwork.hh"
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <system_error>
#include <thread>

using namespace rapidjson;

/**
   \todo Handle new nodes. The VectorFieldReconfiguration could add new nodes as well and managing new nodes requires some reconsideration.
 */

MultilayerNetwork::MultilayerNetwork(void)
{
}

MultilayerNetwork::~MultilayerNetwork(void)
{
  for(std::vector<Layer*>::iterator it=mLayers.begin(); it != mLayers.end(); ++it)
  {
    delete (*it);
  }
}

void MultilayerNetwork::addLayer(int layerId)
{
  mLayers.push_back(new Layer(layerId));
}

std::vector<Layer*> MultilayerNetwork::getLayers(void) const
{
  return mLayers;
}

void executeStepsInThread(std::vector<Node*> &nodes)
{
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    (*itNode)->step();
  }
}

void MultilayerNetwork::step(void)
{
  std::vector<std::vector<Node*>> nodeThreadPartition(numberOfCores);

  for(std::vector<int>::iterator itId=mNodeIds.begin(); itId != mNodeIds.end(); ++itId)
  {
    int id = (*itId);
    int partitionSize = mNodeIds.size()/numberOfCores;
    int index = std::min(numberOfCores-1,id/partitionSize);
    nodeThreadPartition[index].push_back(mNodesMap[id]);
  }

  std::thread stepThreads[numberOfCores];
  for(int i=0; i < numberOfCores; ++i)
  {
    stepThreads[i] = std::thread(executeStepsInThread, std::ref(nodeThreadPartition[i]));
  }

  for(int i=0; i < numberOfCores; ++i)
  {
    stepThreads[i].join();
  }
}

void MultilayerNetwork::iterate(int steps)
{
  for(t=0; t<steps; ++t)
  {
    std::cout<<"t="<<t<<std::endl;
    step();

    if((t % (bufferSize-2)) == (bufferSize-2-1))
    {
      save();
      saveState();
      shiftBuffers();
    }
  }
  save();
  saveState();
}

void MultilayerNetwork::save(std::string filename)
{
  if(filename.empty())
  {
    filename.append("generated/multilayerStructure_");
    filename.append(std::to_string(t/bufferSize));
    filename.append(".json");
  }

  Document document;
  Document::AllocatorType& allocator = document.GetAllocator();
  document.SetObject();

  std::vector<Layer*> layers = this->getLayers();

  Value layerArray(kArrayType);
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    Value layerObject(kObjectType);
    layerObject.AddMember("id", (*itLay)->getId(), allocator);
    Value networkArray(kArrayType);
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      Value networkObject(kObjectType);
      networkObject.AddMember("id", (*itNet)->getId(), allocator);
      std::vector<Node*> nodes = (*itNet)->getNodes();
      Value nodeArray(kArrayType);
      for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	Value nodeObject(kObjectType);
	nodeObject.AddMember("id", (*itNode)->getId(), allocator);

	Value textEq;
	int nodeId = (*itNode)->getId();
	std::string strEq = (*itNet)->getNodeDynamicalEquationString(nodeId);

	textEq.SetString(strEq.c_str(), allocator);
	nodeObject.AddMember("DynamicalEquation", textEq, allocator);

	Network* networkAssigned = (*itNode)->getNetworkAssigned();
	if(networkAssigned != NULL)
	{
	  Value networkAssignedObject(kObjectType);
	  networkAssignedObject.AddMember("id", networkAssigned->getId(), allocator);
	  nodeObject.AddMember("NetworkAssigned", networkAssignedObject, allocator);
	}

	Value neighborArray(kArrayType);
	std::vector<Node*> neighbors = (*itNet)->getNodeNeighbors((*itNode)->getId());
	for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
	{
	  Value neighborObject(kObjectType);
	  neighborObject.AddMember("id", (*itNei)->getId(), allocator);
	  neighborArray.PushBack(neighborObject, allocator);
	}
	nodeObject.AddMember("Neighbors", neighborArray, allocator);
	nodeArray.PushBack(nodeObject, allocator);
      }
      networkObject.AddMember("Nodes", nodeArray, allocator);
      networkArray.PushBack(networkObject, allocator);
    }
    layerObject.AddMember("Networks", networkArray, allocator);
    layerArray.PushBack(layerObject, allocator);
  }
  
  Value layersTmp(kObjectType);
  layersTmp.AddMember("Layers", layerArray, allocator);
  document.AddMember("MultilayerNetwork", layersTmp, allocator);

  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  document.Accept(writer);
  //std::cout<<buffer.GetString()<<std::endl;

  std::ofstream file(filename.c_str());
  file<<buffer.GetString();
  file.close();
}


void MultilayerNetwork::load(const char* filename)
{
  std::ifstream file(filename);
  std::string input((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));

  Document document;
  document.Parse(input.c_str());

  std::map<int, Network*> allNetworks; //used to assign nodes to networks

  Value& layerArray = document["MultilayerNetwork"]["Layers"];
  for(SizeType i=0; i<layerArray.Size(); ++i)
  {
    Value& layerObject = layerArray[i];
    this->addLayer(layerObject["id"].GetInt());
    std::vector<Layer*> layers = this->getLayers();
    Layer* layer = layers[layers.size()-1];
    Value& networkArray = layerObject["Networks"];
    for(SizeType ii=0; ii<networkArray.Size(); ++ii)
    {
      Value& networkObject = networkArray[ii];
      layer->addNetwork(networkObject["id"].GetInt());
      std::vector<Network*> networks = layer->getNetworks();
      Network* network = networks[networks.size()-1];
      allNetworks[network->getId()] = network;
      Value& nodeArray = networkObject["Nodes"];
      for(SizeType iii=0; iii<nodeArray.Size(); ++iii)
      {
	Value& nodeObject = nodeArray[iii];
	network->addNode(nodeObject["id"].GetInt());
	network->setDynamicalEquationString(nodeObject["id"].GetInt(), nodeObject["DynamicalEquation"].GetString());
      }
    }
  }

  //adding edges (nodes must exist at this point)
  for(SizeType i=0; i<layerArray.Size(); ++i)
  {
    Value& layerObject = layerArray[i];
    std::vector<Layer*> layers = this->getLayers();
    Layer* layer = layers[i];
    Value& networkArray = layerObject["Networks"];
    for(SizeType ii=0; ii<networkArray.Size(); ++ii)
    {
      Value& networkObject = networkArray[ii];
      std::vector<Network*> networks = layer->getNetworks();
      Network* network = networks[ii];
      Value& nodeArray = networkObject["Nodes"];
      for(SizeType iii=0; iii<nodeArray.Size(); ++iii)
      {
	Value& nodeObject = nodeArray[iii];
	std::vector<Node*> nodes = network->getNodes();
	Node* node = nodes[iii];
	if(nodeObject.HasMember("NetworkAssigned"))
	{
	  Value& networkAssignedObject = nodeObject["NetworkAssigned"];
	  node->setNetworkAssigned(allNetworks[networkAssignedObject["id"].GetInt()]);
	}
	Value& neighborArray = nodeObject["Neighbors"];
	for(SizeType iiii=0; iiii<neighborArray.Size(); ++iiii)
	{
	  Value& neighborObject = neighborArray[iiii];
	  network->addEdge(node->getId(), neighborObject["id"].GetInt());
	}
      }
    }
  }

  loadNodesToAllEquations();
  updateNodesMap();

  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  document.Accept(writer);
}

void MultilayerNetwork::saveState(std::string filename)
{
  if(filename.empty())
  {
    filename.append("generated/nodeStates_");
    filename.append(std::to_string(t/bufferSize));
    filename.append(".bin");
  }

  std::map<int, Node*> nodesMap;
  std::vector<int> nodeIds;
  collectNodes(nodesMap, nodeIds);
  std::sort(nodeIds.begin(), nodeIds.end());

  double** buffer = new double*[nodeIds.size()];
  for(unsigned i=0; i<nodeIds.size(); ++i)
  {
    buffer[i] = new double[bufferSize];
  }

  int indexCounter = 0;
  for(std::vector<int>::iterator itId=nodeIds.begin(); itId != nodeIds.end(); ++itId)
  {
    Node* currentNode = nodesMap[(*itId)];
    currentNode->getValues(buffer[indexCounter]);
    ++indexCounter;
  }

  std::ofstream out(filename.c_str(), std::ios::binary);
  if(out.is_open())
  {
    for(unsigned i=0; i<nodeIds.size(); ++i)
    {
      for(int j=0; j<bufferSize; ++j)
      {
	out.write((char*)&buffer[i][j], sizeof(double));
      }
    }

  }
  out.close();

  // std::cout<<"In"<<std::endl;
  // for(unsigned i=0; i<nodeIds.size(); ++i)
  // {
  //   std::cout<<buffer[i][0]<<" "<<buffer[i][1]<<std::endl;
  // }

  for(unsigned i=0; i<nodeIds.size(); ++i)
  {
    delete [] buffer[i];
  }
  delete [] buffer;
}

void MultilayerNetwork::loadState(const char* filename)
{
  std::map<int, Node*> nodesMap;
  std::vector<int> nodeIds;
  collectNodes(nodesMap, nodeIds);
  std::sort(nodeIds.begin(), nodeIds.end());

  double** buffer = new double*[nodeIds.size()];
  for(unsigned i=0; i<nodeIds.size(); ++i)
  {
    buffer[i] = new double[bufferSize];
  }

  std::ifstream input(filename, std::ios::binary);
  for(unsigned i=0; i<nodeIds.size(); ++i)
    {
      for(int j=0; j<bufferSize; ++j)
      {
	input.read((char*)&buffer[i][j], sizeof(double));
      }
  }
  input.close();

  // std::cout<<"Out"<<std::endl;
  // for(unsigned i=0; i<nodeIds.size(); ++i)
  // {
  //   std::cout<<buffer[i][0]<<" "<<buffer[i][1]<<std::endl;
  // }

  int indexCounter = 0;
  for(std::vector<int>::iterator itId=nodeIds.begin(); itId != nodeIds.end(); ++itId)
  {
    Node* currentNode = nodesMap[(*itId)];
    currentNode->setValues(buffer[indexCounter]);
    ++indexCounter;
  }

  for(unsigned i=0; i<nodeIds.size(); ++i)
  {
    delete [] buffer[i];
  }
  delete [] buffer;
}


void MultilayerNetwork::loadNodesToAllEquations(void)
{
  std::vector<Layer*> layers = this->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      Network* currentNetwork = (*itNet);
      std::vector<Node*> nodes = currentNetwork->getNodes();
      std::map<int, Node*> nodesMap;
      for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode<nodes.end(); ++itNode)
      {
	nodesMap[(*itNode)->getId()] = (*itNode);
      }
      for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode<nodes.end(); ++itNode)
      {
	DynamicalEquation* dynamicalEquation = currentNetwork->getNodeDynamicalEquation((*itNode)->getId());
	dynamicalEquation->loadNodesToEquation(dynamicalEquation->getBaseCalculationNode(), nodesMap);
      }
    }
  }
}

void MultilayerNetwork::updateNodesMap()
{
  this->collectNodes(mNodesMap, mNodeIds);

  /* This is needed so that each node is stepped only once,
   even if it's part of multiple networks*/
  std::sort(mNodeIds.begin(), mNodeIds.end());
  mNodeIds.erase(unique(mNodeIds.begin(), mNodeIds.end()), mNodeIds.end());
}

bool initialConditionsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2)
{
  std::map<int, Node*> nodesMap1;
  std::vector<int> nodeIds1;
  multilayerNetwork1.collectNodes(nodesMap1, nodeIds1);

  std::map<int, Node*> nodesMap2;
  std::vector<int> nodeIds2;
  multilayerNetwork2.collectNodes(nodesMap2, nodeIds2);
  
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

bool dynamicalEquationsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2)
{
  std::map<int, Node*> nodesMap1;
  std::vector<int> nodeIds1;
  multilayerNetwork1.collectNodes(nodesMap1, nodeIds1);

  std::map<int, Node*> nodesMap2;
  std::vector<int> nodeIds2;
  multilayerNetwork2.collectNodes(nodesMap2, nodeIds2);

  std::map<int, Network*> networksMap1;
  std::vector<int> networkIds1;
  multilayerNetwork1.collectNetworks(networksMap1, networkIds1);

  std::map<int, Network*> networksMap2;
  std::vector<int> networkIds2;
  multilayerNetwork2.collectNetworks(networksMap2, networkIds2);
  
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


void MultilayerNetwork::collectNodes(std::map<int, Node*>& nodesMap, std::vector<int>& nodeIds) const
{
  std::vector<Layer*> layers = this->getLayers();
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

void MultilayerNetwork::collectNetworks(std::map<int, Network*>& networksMap, std::vector<int>& networkIds) const
{
  std::vector<Layer*> layers = this->getLayers();
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


void MultilayerNetwork::shiftBuffers(void)
{
  double* tmpBufferOld = new double[bufferSize];
  double* tmpBufferNew = new double[bufferSize];
  std::vector<Layer*> layers = this->getLayers();
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

std::ostream& operator<<(std::ostream& os, const MultilayerNetwork& multilayerNetwork)
{
  os<<"<< operator"<<std::endl;
  std::vector<Layer*> layers = multilayerNetwork.getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    Layer* currentLayer = (*itLay);
    os<<"-Layer "<<currentLayer->getId()<<std::endl;
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      Network* currentNetwork = (*itNet);
      os<<"  --Network "<<currentNetwork->getId()<<std::endl;
      std::vector<Node*> nodes = currentNetwork->getNodes();
      for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	Node* currentNode = (*itNode);
	os<<"    ---Node "<<currentNode->getId()<<": ";
	double* tmpBuffer = new double[bufferSize];
	currentNode->getValues(tmpBuffer);
	for(int i=0; i<bufferSize; ++i)
	{
	  os<<" "<<tmpBuffer[i];
	}
	delete [] tmpBuffer;
	DynamicalEquation* currentDynamicalEquation = currentNetwork->getNodeDynamicalEquation(currentNode->getId());
	if(currentDynamicalEquation != NULL)
	{
	  os<<"       dyn="<<currentDynamicalEquation->evaluate()<<" "<<currentDynamicalEquation->toString()<<std::endl;
	}
	//uncomment to check if the correct networks are assigned to nodes
	std::vector<Network*> networksToNode = currentNode->getNetworks();
	if(networksToNode.size() > 0)
	{
	  std::vector<Node*> neighbors = networksToNode[0]->getNodeNeighbors(currentNode->getId());
	  for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
	  {
	    os<<"       "<<(*itNei)->getId()<<std::endl;
	  }
	}
	else
	{
	  os<<"       /no net assigned"<<std::endl;
	}
      }
    }
  }
  return os;
}


bool operator==(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2)
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
      if(!(*currentLayer == *correspondingLayer))
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
