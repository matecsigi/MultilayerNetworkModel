#include <iostream>
#include "DownwardInfluenceImpl.hh"

void DownwardInfluenceImpl::calculateDownwardInfluence()
{
  // std::cout<<"----DownwardInfluence------"<<std::endl;
  double change = 0.0;
  double* tmpBuffer = new double[bufferSize];

  std::vector<Network*> networks = mNode->getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    Node* nodeHigher = (*itNet)->getNodeAssigned();
    if(nodeHigher != NULL)
    {
      std::vector<Node*> nodes = (*itNet)->getNodes();
      nodeHigher->getValues(tmpBuffer);
      double changeLocal = tmpBuffer[getIndexTMinusOne(t)]-tmpBuffer[getIndexTMinusTwo(t)];

      changeLocal -= nodeHigher->getChangeByUpwardInfluence((t-1)%2);

      /**
	 The change coming from the upper node through downward influence is distributed equally 
	 among the nodes of the lower network.
      */
      change += changeLocal/nodes.size();
    }
  }

  state_type x = {mNode->getCurrentState()+change};
  mNode->setCurrentState(x);

  mNode->setChangeByDownwardInfluence(t%2, change);

  delete [] tmpBuffer;
}
