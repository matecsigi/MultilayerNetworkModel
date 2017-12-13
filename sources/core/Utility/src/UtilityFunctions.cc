#include "UtilityFunctions.hh"
#include "GlobalVariables.hh"
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
    }
  }
}


void loadNetworkFromJSON(Network* network, std::string filename, int& nodeIdCounter)
{
  std::ifstream file(filename);
  std::string input((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
  Document document;
  document.Parse(input.c_str());
  
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
