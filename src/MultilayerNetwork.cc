#include "MultilayerNetwork.hh"
#include <iostream>
#include <fstream>
#include <map>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

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
	os<<"    ---Node "<<currentNode->getId()<<endl;
	
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
