#include "UtilityFunctions.hh"
#include "GlobalVariables.hh"
#include "Node.hh"
#include "Trace.hh"
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <boost/filesystem.hpp>

using namespace rapidjson;
using namespace boost::filesystem;

int getIndexT(int t)
{
  return (t%(bufferSize-2))+2;
}

int getIndexTMinusOne(int t)
{
  return (t%(bufferSize-2))+1;
}

int getIndexTMinusTwo(int t)
{
  return t%(bufferSize-2);
}

void setIds(Layer* layer, Network* network)
{
  int maxNetworkId = getMaxNetworkId(layer->getMultilayerNetwork());
  int maxNodeId = getMaxNodeId(layer->getMultilayerNetwork());

  network->setId(maxNetworkId+1);
  std::vector<Node*> nodes = network->getNodes();
  int nodeCounter = 1;
  std::map<int, int> idMap;
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    idMap[(*itNode)->getId()] = maxNodeId+nodeCounter;
    (*itNode)->setId(maxNodeId+nodeCounter);
    ++nodeCounter;
  }

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    DynamicalEquation *dynamicalEquation = network->getNodeDynamicalEquation((*itNode)->getId());
    dynamicalEquation->reassignNodeIds(dynamicalEquation->getBaseCalculationNode(), idMap);
  }
}

int getMaxNetworkId(MultilayerNetwork *multilayerNetwork)
{
  int maxNetworkId = 0;
  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet !=networks.end(); ++itNet)
    {
      if((*itNet)->getId() > maxNetworkId)
      {
	maxNetworkId = (*itNet)->getId();
      }
    }
  }
  return maxNetworkId;
}

int getMaxNodeId(MultilayerNetwork *multilayerNetwork)
{
  int maxNodeId = 0;
  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      std::vector<Node*> nodes = (*itNet)->getNodes();
      for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	if((*itNode)->getId() > maxNodeId)
	{
	  maxNodeId = (*itNode)->getId();
	}
      }
    }
  }
  return maxNodeId;
}

double getValueForId(std::vector<IdValuePair> &pairVector, int id)
{
  for(std::vector<IdValuePair>::iterator itPair=pairVector.begin(); itPair != pairVector.end(); ++itPair)
  {
    if(itPair->mId == id)
    {
      return itPair->mValue;
    }
  }
  return 0;
}

void setValueForId(std::vector<IdValuePair> &pairVector, int id, double value)
{
  for(std::vector<IdValuePair>::iterator itPair=pairVector.begin(); itPair != pairVector.end(); ++itPair)
  {
    if(itPair->mId == id)
    {
      itPair->mValue = value;
      return;
    }
  }
  pairVector.push_back(IdValuePair(id, value));
}

VectorFieldPoint* findCorrespondingPoint(VectorFieldPoint* point, VectorField* vectorField)
{
  std::vector<IdValuePair> coordinate = point->getCoordinate();

  std::vector<VectorFieldPoint*> points = vectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=points.begin(); itPoint != points.end(); ++itPoint)
  {
    std::vector<IdValuePair> currentCoordinate = (*itPoint)->getCoordinate();
    bool matchingPoint = true;
    for(std::vector<IdValuePair>::iterator itCord=coordinate.begin(); itCord != coordinate.end(); ++itCord)
    {
      int id = itCord->mId;
      double value = itCord->mValue;
      double currentValue = getValueForId(currentCoordinate, id);
      if(value != currentValue)
      {
	matchingPoint = false;
      }
    }
    if(matchingPoint)
    {
      return (*itPoint);
    }
  }

  return NULL;
}

void loadNetworkFromJSON(Network* network, std::string filename, int& nodeIdCounter)
{
  std::ifstream file(filename);
  std::string input((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
  Document document;
  document.Parse(input.c_str());

  if(document.HasMember("nodes"))
  {

    /* The first value is the id of the node in the python-generated Barabasi network and 
       the second value is the id of the node is the multilayer structure */
    std::map<int, int> nodeIdMap;
    
    Value& nodeArray = document["nodes"];
    for(SizeType i=0; i<nodeArray.Size(); ++i)
    {
      Value& nodeObject = nodeArray[i];
      network->addNodeById(nodeIdCounter);
      nodeIdMap[nodeObject["id"].GetInt()] = nodeIdCounter;
      ++nodeIdCounter;
    }
    
    Value& edgeArray = document["links"];
    for(SizeType i=0; i<edgeArray.Size(); ++i)
    {
      Value& edgeObject = edgeArray[i];
      int sourceId = edgeObject["source"].GetInt();
      int targetId = edgeObject["target"].GetInt();
      
      network->addEdge(nodeIdMap[sourceId], nodeIdMap[targetId]);
      network->addEdge(nodeIdMap[targetId], nodeIdMap[sourceId]);
    }
  }

  else
  {
    Value& networkObject = document["Network"];
    if(networkObject.HasMember("id"))
    {
      network->setId(networkObject["id"].GetInt());
    }

    Value& nodeArray = document["Network"]["Nodes"];
    for(SizeType i=0; i<nodeArray.Size(); ++i)
    {
      Value& nodeObject = nodeArray[i];
      network->addNodeById(nodeObject["id"].GetInt());
      network->setDynamicalEquationString(nodeObject["id"].GetInt(), nodeObject["DynamicalEquation"].GetString());
    }
    for(SizeType i=0; i<nodeArray.Size(); ++i)
    {
      Value& nodeObject = nodeArray[i];
      Value& neighborArray = nodeObject["Neighbors"];

      std::map<int, Node*> nodesMap;
      nodesMap[nodeObject["id"].GetInt()] = network->getNodeById(nodeObject["id"].GetInt());

      for(SizeType ii=0; ii<neighborArray.Size(); ++ii)
      {
	Value& neighborObject = neighborArray[ii];
	network->addEdge(nodeObject["id"].GetInt(), neighborObject["id"].GetInt());
	nodesMap[neighborObject["id"].GetInt()] = network->getNodeById(neighborObject["id"].GetInt());
      }
      DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(nodeObject["id"].GetInt());
      nodeEquation->loadNodesToEquation(nodeEquation->getBaseCalculationNode(), nodesMap);
    }
  }
}

void saveNetworkToJSON(Network* network, std::string filename)
{
  Document document;
  Document::AllocatorType& allocator = document.GetAllocator();
  document.SetObject();

  Value nodesArray(kArrayType);
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Value nodeObject(kObjectType);
    nodeObject.AddMember("id", (*itNode)->getId(), allocator);

    Value textEq;
    std::string strEq = network->getNodeDynamicalEquationString((*itNode)->getId());

    textEq.SetString(strEq.c_str(), allocator);
    nodeObject.AddMember("DynamicalEquation", textEq, allocator);

    Value neighborArray(kArrayType);
    std::vector<Node*> neighbors = network->getNodeNeighbors((*itNode)->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      Value neighborObject(kObjectType);
      neighborObject.AddMember("id", (*itNei)->getId(), allocator);
      neighborArray.PushBack(neighborObject, allocator);
    }
    nodeObject.AddMember("Neighbors", neighborArray, allocator);
    nodesArray.PushBack(nodeObject, allocator);
  }
  Value nodesTmp(kObjectType);
  nodesTmp.AddMember("id", network->getId(), allocator);
  nodesTmp.AddMember("Nodes", nodesArray, allocator);
  document.AddMember("Network", nodesTmp, allocator);

  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  document.Accept(writer);

  std::ofstream file(filename.c_str());
  file<<buffer.GetString();
  file.close();
}


void loadPopulation(Network* network, int control, std::string folderName)
{
  path dir(folderName.c_str());

  int numberOfJsonFiles = 0;
  if(is_directory(dir))
  {
    for(directory_iterator it{dir}; it != directory_iterator{}; ++it)
    {
      std::ostringstream oss;
      oss<<*it;
      std::string fileName = oss.str();
      if(fileName.find(".json") != std::string::npos)
      {
	++numberOfJsonFiles;
      }
    }
  }

  int index = control%numberOfJsonFiles;
  int counter = 0;
  if(is_directory(dir))
  {
    for(directory_iterator it{dir}; it != directory_iterator{}; ++it)
    {
      std::ostringstream oss;
      oss<<*it;
      std::string fileName = oss.str();
      if(fileName.find(".json") != std::string::npos)
      {
	if(counter == index)
	{
	  int tmp = 1;
	  fileName.erase(std::remove(fileName.begin(), fileName.end(), '"'), fileName.end());
	  // std::cout<<"file="<<fileName<<std::endl;
	  loadNetworkFromJSON(network, fileName, tmp);
	  return;
	}
	++counter;
      }
    }
  }

}

void printDirection(std::vector<IdValuePair> direction)
{
  std::cout<<"DIRECTION"<<std::endl;
  for(std::vector<IdValuePair>::iterator itDir=direction.begin(); itDir != direction.end(); ++itDir)
  {
    std::cout<<" "<<itDir->mId<<" -> "<<itDir->mValue<<std::endl;
  }
  std::cout<<"----------------"<<std::endl;
}
