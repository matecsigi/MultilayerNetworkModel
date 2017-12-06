#include <iostream>
#include "UpwardInfluenceImpl.hh"

void UpwardInfluenceImpl::calculateUpwardInfluence()
{
  // std::cout<<"UpwardInfluence"<<std::endl;
  Network* networkAssigned = mNode->getNetworkAssigned();
  if(networkAssigned == NULL)
  {
    return;
  }

  double change = 0.0;
  double* tmpBuffer = new double[bufferSize];
  std::vector<Node*> lowerNodes = networkAssigned->getNodes();
  for(std::vector<Node*>::iterator itNode=lowerNodes.begin(); itNode != lowerNodes.end(); ++itNode)
  {
    Node* currentLowerNode = (*itNode);
    currentLowerNode->getValues(tmpBuffer);
    change += tmpBuffer[getIndexTMinusOne(t)]-tmpBuffer[getIndexTMinusTwo(t)];
    change -= currentLowerNode->getLastChangeByDownwardInfluence();
  }

  state_type x = {mNode->getCurrentState()+change};
  mNode->setCurrentState(x);
}
