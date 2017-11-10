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

//#include <sstream>
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/json_parser.hpp>
//#include <boost/foreach.hpp>

using namespace std;
using namespace rapidjson;
//using boost::property_tree::ptree;
//using boost::property_tree::read_json;
//using boost::property_tree::write_json;

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

ostream& operator<<(ostream& os, const MultilayerNetwork& multilayerNetwork)
{
  os<<"<< operator"<<endl;
  std::vector<Layer*> layers = multilayerNetwork.getLayers();
  for(std::vector<Layer*>::iterator it1=layers.begin(); it1 != layers.end(); ++it1)
  {
    Layer* currentLayer = (*it1);
    os<<"-Layer "<<currentLayer->getId()<<endl;
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      Network* currentNetwork = (*it2);
      os<<"  --Network "<<currentNetwork->getId()<<endl;
      std::vector<Node*> nodes = currentNetwork->getNodes();
      for(std::vector<Node*>::iterator it3 = nodes.begin(); it3 != nodes.end(); ++it3)
      {
	Node* currentNode = (*it3);
	os<<"    ---Node "<<currentNode->getId()<<": ";
	double* tmpBuffer = new double[2];
	currentNode->getValues(tmpBuffer);
	os<<tmpBuffer[0]<<", "<<tmpBuffer[1]<<endl;
	delete [] tmpBuffer;
	//uncomment to check if the correct networks are assigned to nodes
	std::vector<Network*> networksToNode = currentNode->getNetworks();
	if(networksToNode.size() > 0)
	{
	  std::vector<Node*> neighbors = networksToNode[0]->getNodeNeighbors(currentNode->getId());
	  for(std::vector<Node*>::iterator it4=neighbors.begin(); it4 != neighbors.end(); ++it4)
	  {
	    os<<"       "<<(*it4)->getId()<<endl;
	  }
	}
	else
	{
	  os<<"       /no net assigned"<<endl;
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

bool initialConditionsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2)
{
  std::map<int, Node*> nodesMap1;
  std::vector<int> nodeIds1;
  multilayerNetwork1.collectNodes(nodesMap1, nodeIds1);

  std::map<int, Node*> nodesMap2;
  std::vector<int> nodeIds2;
  multilayerNetwork2.collectNodes(nodesMap2, nodeIds2);
  
  double* tmpBuffer1 = new double[2];
  double* tmpBuffer2 = new double[2];
  for(std::vector<int>::iterator itId=nodeIds1.begin(); itId != nodeIds1.end(); ++itId)
  {
    Node* node1 = nodesMap1[(*itId)];
    Node* node2 = nodesMap2[(*itId)];
    node1->getValues(tmpBuffer1);
    node2->getValues(tmpBuffer2);
    for(int i=0; i<2; ++i)
    {
      if(tmpBuffer1[i] != tmpBuffer2[i])
      {
	return false;
      }
    }
  }
  
  return true;
}

void MultilayerNetwork::save(const char* filename)
{
  Document document;
  Document::AllocatorType& allocator = document.GetAllocator();
  document.SetObject();

  std::vector<Layer*> layers = this->getLayers();

  Value layerArray(kArrayType);
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    Value layerObject(kObjectType);
    layerObject.AddMember("id", (*it)->getId(), allocator);
    Value networkArray(kArrayType);
    std::vector<Network*> networks = (*it)->getNetworks();
    for(std::vector<Network*>::iterator it2=networks.begin(); it2 != networks.end(); ++it2)
    {
      Value networkObject(kObjectType);
      networkObject.AddMember("id", (*it2)->getId(), allocator);
      std::vector<Node*> nodes = (*it2)->getNodes();
      Value nodeArray(kArrayType);
      for(std::vector<Node*>::iterator it3=nodes.begin(); it3 != nodes.end(); ++it3)
      {
	Value nodeObject(kObjectType);
	nodeObject.AddMember("id", (*it3)->getId(), allocator);

	Network* networkAssigned = (*it3)->getNetworkAssigned();
	if(networkAssigned != NULL)
	{
	  Value networkAssignedObject(kObjectType);
	  networkAssignedObject.AddMember("id", networkAssigned->getId(), allocator);
	  nodeObject.AddMember("NetworkAssigned", networkAssignedObject, allocator);
	}

	Value neighborArray(kArrayType);
	std::vector<Node*> neighbors = (*it2)->getNodeNeighbors((*it3)->getId());
	for(std::vector<Node*>::iterator it4=neighbors.begin(); it4 != neighbors.end(); ++it4)
	{
	  Value neighborObject(kObjectType);
	  neighborObject.AddMember("id", (*it4)->getId(), allocator);
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

  std::ofstream file(filename);
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
	  node->assignToNetwork(allNetworks[networkAssignedObject["id"].GetInt()]);
	}
	Value& neighborArray = nodeObject["Neighbors"];
	for(SizeType iiii=0; iiii<neighborArray.Size(); ++iiii)
	{
	  Value& neighborObject = neighborArray[iiii];
	  network->addEdge(network->getLocalId(node->getId()), network->getLocalId(neighborObject["id"].GetInt()));
	}
      }
    }
  }

  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  document.Accept(writer);
  //std::cout<<buffer.GetString()<<std::endl;
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

void MultilayerNetwork::saveState(const char* filename)
{
  std::map<int, Node*> nodesMap;
  std::vector<int> nodeIds;
  collectNodes(nodesMap, nodeIds);
  std::sort(nodeIds.begin(), nodeIds.end());

  double** buffer = new double*[nodeIds.size()];
  for(unsigned i=0; i<nodeIds.size(); ++i)
  {
    buffer[i] = new double[2];
  }

  int indexCounter = 0;
  for(std::vector<int>::iterator itId=nodeIds.begin(); itId != nodeIds.end(); ++itId)
  {
    Node* currentNode = nodesMap[(*itId)];
    currentNode->getValues(buffer[indexCounter]);
    ++indexCounter;
  }

  ofstream out(filename, ios::binary);
  if(out.is_open())
  {
    for(unsigned i=0; i<nodeIds.size(); ++i)
    {
      for(int j=0; j<2; ++j)
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

  for(int i=0; i<10; ++i)
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
    buffer[i] = new double[2];
  }

  ifstream input(filename, ios::binary);
  for(unsigned i=0; i<nodeIds.size(); ++i)
    {
      for(int j=0; j<2; ++j)
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

  for(int i=0; i<10; ++i)
  {
    delete [] buffer[i];
  }
  delete [] buffer;
}
