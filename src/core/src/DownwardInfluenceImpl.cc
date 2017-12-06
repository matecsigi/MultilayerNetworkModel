#include <iostream>
#include "DownwardInfluenceImpl.hh"

void DownwardInfluenceImpl::calculateDownwardInfluence()
{
  // std::cout<<"DownwardInfluence"<<std::endl;
  double change = 0.0;
  double* tmpBuffer = new double[bufferSize];

  std::vector<Network*> networks = mNode->getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    Node* nodeHigher = (*itNet)->getNodeAssigned();
    if(nodeHigher != NULL)
    {
      nodeHigher->getValues(tmpBuffer);
      change += tmpBuffer[getIndexTMinusOne(t)]-tmpBuffer[getIndexTMinusTwo(t)];
    }
  }
  change -= mNode->getLastChangeByUpwardInfluence();

  state_type x = {mNode->getCurrentState()+change};
  mNode->setCurrentState(x);

  delete [] tmpBuffer;
}
