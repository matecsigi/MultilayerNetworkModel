#include <iostream>
#include "DownwardInfluenceImpl.hh"

double DownwardInfluenceImpl::calculateDownwardInfluence()
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
      change += tmpBuffer[(t+1)%bufferSize]-tmpBuffer[(t)%bufferSize];
      // std::cout<<tmpBuffer[(t+1)%bufferSize]<<"-"<<tmpBuffer[(t)%bufferSize]<<"="<<change<<std::endl;
    }
  }
  state_type x = {mNode->getCurrentState()+change};

  delete [] tmpBuffer;

  return 1.9;
}
