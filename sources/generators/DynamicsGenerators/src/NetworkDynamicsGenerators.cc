#include "NetworkDynamicsGenerators.hh"
#include "Network.hh"
#include "Node.hh"

void linearNetworkDynamicsGenerator(Network* network)
{
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* currentNode = (*itNode);
    std::string myString;
    myString.append("-");
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
    // myString.append("*0.2*ID");
    // myString.append(std::to_string(neighbors[0]->getId()));
    network->setDynamicalEquationString(currentNode->getId(), myString);
  }
}
