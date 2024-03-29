#include "UpwardInfluenceImpl.hh"
#include "Node.hh"
#include "Network.hh"
#include "MultilayerNetwork.hh"
#include "UtilityFunctions.hh"
#include "OdeWrapper.hh"
#include <boost/numeric/odeint.hpp>
#include <iostream>

using namespace boost::numeric::odeint;

void UpwardInfluenceImpl::calculateUpwardInfluence(double upwardMultiplier)
{
  int t = mNode->getTime();
  // std::cout<<"---UpwardInfluence---"<<std::endl;
  if(t == 0){return;}

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

    // std::cout<<"  +"<<tmpBuffer[getIndexTMinusOne(t)]-tmpBuffer[getIndexTMinusTwo(t)]<<" == "<<tmpBuffer[getIndexTMinusOne(t)]<<" -- "<<tmpBuffer[getIndexTMinusTwo(t)]<<" ... "<<currentLowerNode->getId()<<std::endl;

    change -= currentLowerNode->getChangeByDownwardInfluence((t-1)%2);
    // std::cout<<"  -"<<currentLowerNode->getChangeByDownwardInfluence((t-1)%2)<<std::endl;
  }
  change = change*upwardMultiplier;

  state_type x = {mNode->getCurrentState()+change};
  mNode->setCurrentState(x[0]);

  // std::cout<<"Upward:"<<mNode->getId()<<"->"<<change<<std::endl;

  mNode->setChangeByUpwardInfluence(t%2, change);

  delete [] tmpBuffer;
}
