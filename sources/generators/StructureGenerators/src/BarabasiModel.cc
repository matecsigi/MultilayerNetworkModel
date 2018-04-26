#include "BarabasiModel.hh"
#include "Node.hh"
#include "NetworkClassUtility.hh"

void generateNetwork(Network* network, int numberOfNodes, int m)
{
  for(int i=0; i<numberOfNodes; ++i)
  {
    std::vector<Node*> nodes = network->getNodes();
    Node* newNode = network->addNode();
    for(int j=0; j<std::min(m, (int)nodes.size()); ++j)
    {
      Node* node = chooseNodePreferential(newNode, network);
      network->addEdge(newNode->getId(), node->getId());
      network->addEdge(node->getId(), newNode->getId());
    }
  }
}

Node* chooseNodePreferential(Node* newNode, Network* network)
{
  std::vector<Node*> nodes = network->getNodes();
  std::vector<Node*> neighbors = network->getNodeNeighbors(newNode->getId());
  for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
  {
    nodes.erase(std::remove(nodes.begin(), nodes.end(), (*itNei)), nodes.end());
  }
  nodes.erase(std::remove(nodes.begin(), nodes.end(), newNode), nodes.end());
  
  int sumDegree = 0;
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    sumDegree += getNodeDegree(network, (*itNode)->getId());
  }

  double random = (rand()/RAND_MAX)*sumDegree;
  double counter = 0;
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    counter += getNodeDegree(network, (*itNode)->getId());
    if(counter >= random)
    {
      return (*itNode);
    }
  }

  return NULL;
}
