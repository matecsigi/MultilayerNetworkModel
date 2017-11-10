#include "InitialConditionGeneratorImpl.hh"

void InitialConditionGeneratorImpl::generateInitialCondition()
{
  double* tmpBuffer = new double[2];
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
	tmpBuffer[0] = 1.1;
	tmpBuffer[1] = 1.3;
	currentNode->setValues(tmpBuffer);
      }
    }
  }
  delete [] tmpBuffer;
}
