#include "SerializedClasses.hh"
#include "Node.hh"

void serializeNetwork(Network *network, SerializedNetwork *serializedNetwork)
{
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* currentNode = (*itNode);
    int id = currentNode->getId();
    std::string dynamicalEquation = network->getNodeDynamicalEquationString(id);
    serializedNetwork->mNodes.push_back(SerializedNode(id, dynamicalEquation));
    
    std::vector<Node*> neighbors = network->getNodeNeighbors(id);
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      int neighborId = (*itNei)->getId();
      serializedNetwork->mEdges.push_back(SerializedEdge(id, neighborId));
    }
  }
}

void deserializeNetwork(SerializedNetwork* serializedNetwork, Network* network)
{

}
