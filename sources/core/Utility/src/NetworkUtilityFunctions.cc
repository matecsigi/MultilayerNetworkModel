#include "NetworkUtilityFunctions.hh"

void copyNetwork(Network* oldNetwork, Network* newNetwork)
{
  newNetwork->removeAllEdges();
  
  std::vector<Node*> nodes = oldNetwork->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* oldNode = (*itNode);
    if(newNetwork->getNodeById(oldNode->getId()) == NULL)
    {
      newNetwork->addNode(oldNode->getId());      
    }
  }

  // double* tmpBuffer = new double[bufferSize];

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
    // oldNode->getValues(tmpBuffer);
    // newNode->setValues(tmpBuffer);

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
  // delete [] tmpBuffer; 
}

std::vector<IdValuePair> calculateLowerNetworkDirection(Node *node)
{
  Network* networkAssigned = node->getNetworkAssigned();
  std::vector<IdValuePair> currentState = networkAssigned->getCurrentState();
  return  networkAssigned->getDirectionAtState(currentState);
}

std::vector<IdValuePair> calculateHigherNetworksDirection(Node *node)
{
  // std::cout<<std::endl;
  // std::cout<<"====HigherDirections==="<<std::endl;
  std::vector<std::vector<IdValuePair>> directionsInAllHigherNetworks;
  std::vector<Network*> networks = node->getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    std::vector<IdValuePair> currentState = (*itNet)->getCurrentState();
    // std::cout<<"higherNetworkDirection"<<std::endl;
    // printDirection((*itNet)->getDirectionAtState(currentState));
    directionsInAllHigherNetworks.push_back((*itNet)->getDirectionAtState(currentState));
  }

  std::vector<IdValuePair> referenceHigherDirection = directionsInAllHigherNetworks[0];
  // std::cout<<"referenceHigherDirection"<<std::endl;
  // printDirection(referenceHigherDirection);

  std::vector<IdValuePair> sumDirectionInHigherNetworks;
  for(std::vector<IdValuePair>::iterator itDir=referenceHigherDirection.begin(); itDir != referenceHigherDirection.end(); ++itDir)
  {
    int key = itDir->mId;
    double sumDir = 0;
    for(std::vector<std::vector<IdValuePair>>::iterator itNetD=directionsInAllHigherNetworks.begin(); itNetD != directionsInAllHigherNetworks.end(); ++itNetD)
    {
      // std::cout<<">>"<<key<<" -> "<<getValueForId((*itNetD), key)<<std::endl;
      sumDir += getValueForId((*itNetD), key);
    }
    setValueForId(sumDirectionInHigherNetworks, key, sumDir);
  }
  // printDirection(sumDirectionInHigherNetworks);
  // std::cout<<"======================"<<std::endl;
  return sumDirectionInHigherNetworks;
}
