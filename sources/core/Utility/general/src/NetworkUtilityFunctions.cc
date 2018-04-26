#include "NetworkUtilityFunctions.hh"

void copyNetwork(Network* oldNetwork, Network* newNetwork)
{
  newNetwork->removeAllEdges();
  newNetwork->setId(oldNetwork->getId());

  newNetwork->setTime(oldNetwork->getTime());

  std::vector<Node*> nodes = oldNetwork->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* oldNode = (*itNode);
    if(newNetwork->getNodeById(oldNode->getId()) == NULL)
    {
      newNetwork->addNodeById(oldNode->getId());      
    }
  }

  double* tmpBuffer = new double[bufferSize];

  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* oldNode = (*itNode);
    std::vector<Node*> neighbors = oldNetwork->getNodeNeighbors(oldNode->getId());
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      Node* oldNeighbor = (*itNei);
      newNetwork->addEdge(oldNode->getId(), oldNeighbor->getId());
    }
    Node* newNode = newNetwork->getNodeById(oldNode->getId());
    oldNode->getValues(tmpBuffer);
    newNode->setValues(tmpBuffer);

    std::string strEquation = oldNetwork->getNodeDynamicalEquationString(oldNode->getId());
    newNetwork->setDynamicalEquationString(oldNode->getId(), strEquation);
    DynamicalEquation* nodeEquation = newNetwork->getNodeDynamicalEquation(newNode->getId());
    std::vector<Node*> newNeighbors = newNetwork->getNodeNeighbors(newNode->getId());
    // nodes.push_back(newNode);
    std::map<int, Node*> nodesMap;
    nodesMap[newNode->getId()] = newNode;
    for(std::vector<Node*>::iterator itNewN=newNeighbors.begin(); itNewN != newNeighbors.end(); ++itNewN)
    {
      nodesMap[(*itNewN)->getId()] = *itNewN;
    }
    nodeEquation->loadNodesToEquation(nodeEquation->getBaseCalculationNode(), nodesMap);
  }
  delete [] tmpBuffer; 
}

std::vector<IdValuePair> getIsolatedDirectionAtState(Network* network, std::vector<IdValuePair> &basePointCoordinates)
{
  std::vector<IdValuePair> directions;
  std::vector<IdValuePair> finalState;
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::const_iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation((*itNode)->getId());
    // std::cout<<nodeEquation->toString()<<std::endl;
    (*itNode)->stepOdeAtState(nodeEquation, basePointCoordinates, finalState);
  }
  
  for(std::vector<IdValuePair>::iterator itState=basePointCoordinates.begin(); itState != basePointCoordinates.end(); ++itState)
   {
     int id = itState->mId;
     double startValue = getValueForId(basePointCoordinates, id);
     double finalValue = getValueForId(finalState, id);
     setValueForId(directions, id, finalValue-startValue);
   }

  return directions;
}

std::vector<IdValuePair> getEnvironmentalDirectionAtState(Network* network, std::vector<IdValuePair> &basePointCoordinates)
{
  std::vector<IdValuePair> directions;
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  Network* insertedNetwork = createEnvironmentalMultilayerNetwork(multilayerNetwork, network);

  SimulationParameterContainer *parameters = new SimulationParameterContainer;
  parameters->runVectorFieldReconfiguration = false;  
  insertedNetwork->setState(basePointCoordinates);
  multilayerNetwork->iterate(2, parameters);

  std::vector<IdValuePair> finalState = insertedNetwork->getState();

  for(std::vector<IdValuePair>::iterator itState=basePointCoordinates.begin(); itState != basePointCoordinates.end(); ++itState)
   {
     int id = itState->mId;
     double startValue = getValueForId(basePointCoordinates, id);
     double finalValue = getValueForId(finalState, id);
     setValueForId(directions, id, finalValue-startValue);
   }

  delete multilayerNetwork;
  delete parameters;
  return directions;
}

Network* createEnvironmentalMultilayerNetwork(MultilayerNetwork* multilayerNetwork, Network* network)
{
  multilayerNetwork->setTime(network->getTime());

  Node* nodeAssigned = network->getNodeAssigned();
  std::vector<Network*> higherNetworks = nodeAssigned->getNetworks();
  Network* higherNetwork = higherNetworks[0];

  Layer* layer1 = multilayerNetwork->addLayer();
  Layer* layer2 = multilayerNetwork->addLayer();
  Network* insertedHigherNetwork = layer1->insertNetwork(higherNetwork);

  Network* returnNetwork = NULL;
  std::vector<Node*> nodes = higherNetwork->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* node = insertedHigherNetwork->getNodeById((*itNode)->getId());
    Network* lowerNetwork = (*itNode)->getNetworkAssigned();
    Network* insertedNetwork = layer2->insertNetwork(lowerNetwork);
    node->setNetworkAssigned(insertedNetwork);
    if(lowerNetwork->getId() == network->getId())
    {
      returnNetwork = insertedNetwork;
    }
  }
  
  return returnNetwork;
}