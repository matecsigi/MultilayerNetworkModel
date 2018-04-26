#include "NetworkClassUtility.hh"
#include "NodeClassUtility.hh"
#include "Network.hh"
#include "Node.hh"

void loadNodesToEquations(Network *network)
{
  std::vector<Node*> nodes = network->getNodes();
  std::map<int, Node*> nodesMap;
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    nodesMap[(*itNode)->getId()] = (*itNode);
  }
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    DynamicalEquation *dynamicalEquation = network->getNodeDynamicalEquation((*itNode)->getId());
    dynamicalEquation->loadNodesToEquation(dynamicalEquation->getBaseCalculationNode(), nodesMap);
  }
}

int getNodeDegree(Network *network, int nodeId)
{
  std::vector<Node*> neighbors = network->getNodeNeighbors(nodeId);
  return neighbors.size();
}

bool networksEqual(Network& network1, Network& network2)
{
  std::vector<Node*> nodes1 = network1.getNodes();
  std::vector<Node*> nodes2 = network2.getNodes();
  if(nodes1.size() != nodes2.size())
  {
    return false;
  }

  std::map<int, Node*> nodeMap;
  for(std::vector<Node*>::iterator itNode=nodes1.begin(); itNode != nodes1.end(); ++itNode)
  {
    nodeMap[(*itNode)->getId()] = (*itNode);
  }

  for(std::vector<Node*>::iterator itNode=nodes2.begin(); itNode != nodes2.end(); ++itNode)
  {
    if(nodeMap.count((*itNode)->getId()))
    {
      Node* currentNode = (*itNode);
      Node* correspondingNode = nodeMap[currentNode->getId()];
      if(!(nodesEqual(*currentNode, *correspondingNode)))
      {
	return false;
      }
      
      std::vector<Node*> neighbors1 = network1.getNodeNeighbors(currentNode->getId());
      std::vector<Node*> neighbors2 = network2.getNodeNeighbors(currentNode->getId());
      if(neighbors1.size() != neighbors2.size())
      {
	return false;
      }

      std::map<int, Node*> neighborMap;
      for(std::vector<Node*>::iterator itNei=neighbors1.begin(); itNei != neighbors1.end(); ++itNei)
      {
	neighborMap[(*itNei)->getId()] = (*itNei);
      }

      for(std::vector<Node*>::iterator itNei=neighbors2.begin(); itNei != neighbors2.end(); ++itNei)
      {
	if(!(neighborMap.count((*itNei)->getId())))
	{
	  return false;
	}
      }
    }
    else
    {
      return false;
    }
  }

  return true;
}
