#include "DynamicalEquationGeneratorSimpleImpl.hh"

void DynamicalEquationGeneratorSimpleImpl::generateDynamicalEquations()
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    Layer* currentLayer = (*itLay);
    std::vector<Network*> networks = currentLayer->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      Network* currentNetwork = (*itNet);
      std::vector<Node*> nodes = currentNetwork->getNodes();
      for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
      {
	Node* currentNode = (*itNode);
	std::string myString = "2*(1+1)";
	currentNetwork->setDynamicalEquation(currentNode->getId(), myString);
      }
    }
  }
}
