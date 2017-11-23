#include "Node.hh"
#include <iostream>
#include "UpwardInfluenceImpl.hh"
#include "DownwardInfluenceImpl.hh"
#include <boost/numeric/odeint.hpp>

#include "OdeWrapper.hh"

using namespace std;
using namespace boost::numeric::odeint;

Node::Node(void)
{
  mNetworkAssigned = NULL;
  mValues = new double[bufferSize];
  mUpwardInfluence = new UpwardInfluenceImpl();
  mDownwardInfluence = new DownwardInfluenceImpl();
}

Node::Node(int id)
{
  mNodeId = id;
  mNetworkAssigned = NULL;
  mValues = new double[bufferSize];
  mUpwardInfluence = new UpwardInfluenceImpl();
  mDownwardInfluence = new DownwardInfluenceImpl();
}

Node::~Node(void)
{
  delete [] mValues;
  if(mUpwardInfluence != NULL)
  {
    delete mUpwardInfluence;
  }
  if(mDownwardInfluence != NULL)
  {
    delete mDownwardInfluence;
  }
}

void Node::step(void)
{
  cout<<"Stepping node "<<mNodeId<<endl;
  mUpwardInfluence->calculateUpwardInfluence();
  mDownwardInfluence->calculateDownwardInfluence();
  std::vector<Network*> networks = getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    DynamicalEquation* dynamicalEquation = (*itNet)->getNodeDynamicalEquation(this->getId());
    stepODE(dynamicalEquation);
  }
}

void Node::stepODE(DynamicalEquation* dynamicalEquation)
{
  // std::cout<<"stepODE "<<std::endl;
  state_type x = {getPreviousState()};

  std::cout<<"start="<<x[0]<<std::endl;

  OdeWrapper wrapper(dynamicalEquation);
  integrate(wrapper, x, 0.0, 15.0, 0.1);
  
  std::cout<<"   x="<<x[0]<<std::endl;
  setCurrentState(x);
}

void Node::addToNetwork(Network* networkPtr)
{
  mNetworks.push_back(networkPtr);
}

void Node::assignToNetwork(Network* network)
{
  mNetworkAssigned = network;
  network->assignToNode(this);

}

void Node::setValues(double* values)
{
  for(int i=0; i<bufferSize; ++i)
  {
    mValues[i] = values[i];
  }
}

void Node::setUpwardInfluence()
{
  mUpwardInfluence = new UpwardInfluenceImpl();
}

void Node::setDownwardInfluence()
{
  mDownwardInfluence = new DownwardInfluenceImpl();
}

void Node::setInitialConditions(double* values)
{
  for(int i=0; i<initialConditionsSize; ++i)
  {
    mValues[i] = values[i];
  }  
}

void Node::setCurrentState(state_type state)
{
  mValues[t+2] = state[0];
}

int Node::getId(void) const
{
  return mNodeId;
}

Network* Node::getNetworkAssigned(void) const
{
  return mNetworkAssigned;
}

std::vector<Network*> Node::getNetworks(void) const
{
  return mNetworks;
}

void Node::getValues(double* values)
{
  for(int i=0; i<bufferSize; ++i)
  {
    values[i] = mValues[i];
  }
}

double Node::getValue(void)
{
  return mValues[0];
}

double Node::getPreviousState()
{
  return mValues[t+1];
}

bool operator==(const Node& node1, const Node& node2)
{
  Network* networkAssigned1 = node1.getNetworkAssigned();
  Network* networkAssigned2 = node2.getNetworkAssigned();
  
  if((networkAssigned1 == NULL) && (networkAssigned2 == NULL))
  {
    return true;
  }
  else if((networkAssigned1 != NULL) && (networkAssigned2 == NULL))
  {
    return false;
  }
  else if((networkAssigned1 == NULL) && (networkAssigned2 != NULL))
  {
    return false;
  }
  else if(networkAssigned1->getId() != networkAssigned2->getId())
  {
    return false;
  }

  return true;
}
