#include "UtilityFunctions.hh"
#include "GlobalVariables.hh"
#include "Node.hh"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

using namespace rapidjson;

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
      network->addNode(nodeIdCounter);
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
    Value& nodeArray = document["Network"]["Nodes"];
    for(SizeType i=0; i<nodeArray.Size(); ++i)
    {
      Value& nodeObject = nodeArray[i];
      network->addNode(nodeObject["id"].GetInt());
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
  nodesTmp.AddMember("Nodes", nodesArray, allocator);
  document.AddMember("Network", nodesTmp, allocator);

  StringBuffer buffer;
  PrettyWriter<StringBuffer> writer(buffer);
  document.Accept(writer);

  std::ofstream file(filename.c_str());
  file<<buffer.GetString();
  file.close();
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
