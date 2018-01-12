#include "NetworkDynamicsGenerators.hh"
#include "Node.hh"

void linearNetworkDynamicsGenerator(Network* network)
{
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* currentNode = (*itNode);
    std::string myString = "-1";
    myString.append("+");
    myString.append("1*ID");
    myString.append((std::to_string(currentNode->getId())));
    
    std::vector<Node*> neighbors = network->getNodeNeighbors((*itNode)->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      myString.append("+");
      myString.append("0.2*");
      myString.append("ID");
      myString.append((std::to_string((*itNei)->getId())));
    }
    network->setDynamicalEquationString(currentNode->getId(), myString);
  }
}
