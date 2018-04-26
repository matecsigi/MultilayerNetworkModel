#include "MultilayerNetwork.hh"
#include "MultilayerNetworkServer.hh"
#include "Trace.hh"
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
#include <boost/mpi.hpp>

using namespace rapidjson;

/**
   \todo Handle new nodes. The VectorFieldReconfiguration could add new nodes as well and managing new nodes requires some reconsideration.
 */

MultilayerNetwork::MultilayerNetwork(void)
{
  mTime = 0;
}

MultilayerNetwork::~MultilayerNetwork(void)
{
  for(std::vector<Layer*>::iterator it=mLayers.begin(); it != mLayers.end(); ++it)
  {
    delete (*it);
  }
}

int MultilayerNetwork::getTime()
{
  return mTime;
}

void MultilayerNetwork::setTime(int time)
{
  mTime = time;
}

Layer* MultilayerNetwork::addLayer()
{
  int maximalLayerId = 0;
  for(std::vector<Layer*>::iterator itLay=mLayers.begin(); itLay != mLayers.end(); ++itLay)
  {
    if((*itLay)->getId() > maximalLayerId)
    {
      maximalLayerId = (*itLay)->getId();
    }
  }
  Layer* newLayer = new Layer(maximalLayerId+1);
  newLayer->setMultilayerNetwork(this);
  mLayers.push_back(newLayer);
  return newLayer;
}

void MultilayerNetwork::addLayerById(int layerId)
{
  int maximalLayerId = 0;
  for(std::vector<Layer*>::iterator itLay=mLayers.begin(); itLay != mLayers.end(); ++itLay)
  {
    if((*itLay)->getId() > maximalLayerId)
    {
      maximalLayerId = (*itLay)->getId();
    }
  }
  Layer *newLayer = new Layer(layerId);
  newLayer->setMultilayerNetwork(this);
  mLayers.push_back(newLayer);
}

std::vector<Layer*> MultilayerNetwork::getLayers(void) const
{
  return mLayers;
}

void executeStepsInThread(std::vector<Node*> &nodes, SimulationParameterContainer *parameters)
{
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    (*itNode)->step(parameters);
  }
}

void MultilayerNetwork::step(SimulationParameterContainer *parameters)
{
  bool deletionNeeded = false;
  if(parameters == NULL){parameters = new SimulationParameterContainer; deletionNeeded = true;};

  MultilayerNetworkServer server;
  if(parameters->cluster == true)
  {
    server.start();
  }

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
    stepThreads[i] = std::thread(executeStepsInThread, std::ref(nodeThreadPartition[i]), parameters);
  }

  for(int i=0; i < numberOfCores; ++i)
  {
    stepThreads[i].join();
  }

  if(parameters->cluster == true)
  {
    server.processQueue(this);
  }

  if(deletionNeeded == true){delete parameters;}
}

void MultilayerNetwork::iterate(int steps, SimulationParameterContainer *parameters, IObserver *observer)
{
  bool deletionNeeded = false;
  if(parameters == NULL){parameters = new SimulationParameterContainer; deletionNeeded = true;};

  updateNodesMap(this);

  if(parameters->cluster == true){calculateClusterMessageSizes(this, parameters);}

  if(observer != NULL){observer->atStart();}
  for(mTime=mTime; mTime<(mTime+steps); ++mTime)
  {
    if(parameters->printTrace)
    {
      traceRun("Time "+std::to_string(mTime)+"\n");
    }
    step(parameters);

    if(observer != NULL){observer->atStep();}

    if((mTime % (bufferSize-2)) == (bufferSize-2-1))
    {
      save();
      saveState();
      shiftBuffers(this);
    }
  }
  if(parameters->printTrace){this->print();}

  save();
  saveState();
  if(observer != NULL){observer->atFinish();}
  if(deletionNeeded == true){delete parameters;}
}

void MultilayerNetwork::save(std::string filename)
{
  if(filename.empty())
  {
    filename.append("generated/multilayerStructure_");
    filename.append(std::to_string(mTime/bufferSize));
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
    this->addLayerById(layerObject["id"].GetInt());
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
	network->addNodeById(nodeObject["id"].GetInt());
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

  updateNodesMap(this);

  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  document.Accept(writer);
}

void MultilayerNetwork::saveState(std::string filename)
{
  if(filename.empty())
  {
    filename.append("generated/nodeStates_");
    filename.append(std::to_string(mTime/bufferSize));
    filename.append(".bin");
  }

  std::map<int, Node*> nodesMap;
  std::vector<int> nodeIds;
  collectNodes(this, nodesMap, nodeIds);
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
  collectNodes(this, nodesMap, nodeIds);
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

void MultilayerNetwork::print()
{
  traceDebug("Multilayer Network\n");
  for(std::vector<Layer*>::iterator itLay=mLayers.begin(); itLay != mLayers.end(); ++itLay)
  {
    traceDebug("  ");
    (*itLay)->print();
  }
}
