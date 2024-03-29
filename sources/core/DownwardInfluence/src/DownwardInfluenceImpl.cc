#include "DownwardInfluenceImpl.hh"
#include <boost/numeric/odeint.hpp>
#include "Node.hh"
#include "Network.hh"
#include "MultilayerNetwork.hh"
#include "UtilityFunctions.hh"
#include <iostream>
#include "OdeWrapper.hh"

using namespace boost::numeric::odeint;

void DownwardInfluenceImpl::calculateDownwardInfluence(double downwardMultiplier)
{
  int t = mNode->getTime();

  // std::cout<<"----DownwardInfluence------"<<std::endl;
  if(t == 0){return;}

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
  change = change*downwardMultiplier;

  state_type x = {mNode->getCurrentState()+change};
  mNode->setCurrentState(x[0]);

  // std::cout<<"Downward:"<<mNode->getId()<<"->"<<change<<std::endl;

  mNode->setChangeByDownwardInfluence(t%2, change);

  delete [] tmpBuffer;
}
