#include "NetworkInitialConditionGenerators.hh"
#include "GlobalVariables.hh"
#include "Network.hh"
#include "Node.hh"

void oneNetworkInitialConditions(Network* network)
{
  double* tmpBuffer = new double[bufferSize];
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* currentNode = (*itNode);
    for(int i=0; i<bufferSize; ++i)
    {
      tmpBuffer[i] = 0;
    }
    for(int i=0; i<initialConditionsSize; ++i)
    {
      tmpBuffer[i] = 1;
    }
    currentNode->setValues(tmpBuffer);
  }

  delete [] tmpBuffer;
}

void randomNetworkInitialConditions(Network* network, double min, double max)
{
  double* tmpBuffer = new double[bufferSize];
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::iterator itNode = nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* currentNode = (*itNode);
    for(int i=0; i<bufferSize; ++i)
    {
      tmpBuffer[i] = 0;
    }
    for(int i=0; i<initialConditionsSize; ++i)
    {
      tmpBuffer[i] = min+((double)rand()/RAND_MAX)*(max-min);
    }
    currentNode->setValues(tmpBuffer);
  }

  delete [] tmpBuffer;  
}
