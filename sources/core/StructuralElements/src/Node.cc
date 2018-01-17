#include "Node.hh"
#include <iostream>
#include "UpwardInfluenceImpl.hh"
#include "DownwardInfluenceImpl.hh"
#include "VectorFieldReconfigurationImpl.hh"
#include "OdeWrapper.hh"
#include "OdeWrapperAtState.hh"
#include <boost/numeric/odeint.hpp>

using namespace boost::numeric::odeint;

Node::Node(void)
{
  mNetworkAssigned = NULL;
  mValues = new double[bufferSize];
  mUpwardInfluence = new UpwardInfluenceImpl(this);
  mDownwardInfluence = new DownwardInfluenceImpl(this);
  mVectorFieldReconfiguration = new VectorFieldReconfigurationImpl(this);
  mChangeByUpwardInfluence.push_back(0.0);
  mChangeByUpwardInfluence.push_back(0.0);
  mChangeByDownwardInfluence.push_back(0.0);
  mChangeByDownwardInfluence.push_back(0.0);
}

Node::Node(int id)
{
  mNodeId = id;
  mNetworkAssigned = NULL;
  mValues = new double[bufferSize];
  mUpwardInfluence = new UpwardInfluenceImpl(this);
  mDownwardInfluence = new DownwardInfluenceImpl(this);
  mVectorFieldReconfiguration = new VectorFieldReconfigurationImpl(this);
  mChangeByUpwardInfluence.push_back(0.0);
  mChangeByUpwardInfluence.push_back(0.0);
  mChangeByDownwardInfluence.push_back(0.0);
  mChangeByDownwardInfluence.push_back(0.0);
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
  if(mVectorFieldReconfiguration != NULL)
  {
    delete mVectorFieldReconfiguration;
  }
}

int Node::getId(void) const
{
  return mNodeId;
}

std::vector<Network*> Node::getNetworks(void) const
{
  return mNetworks;
}

Network* Node::getNetworkAssigned(void) const
{
  return mNetworkAssigned;
}

void Node::setNetworkAssigned(Network* network)
{
  mNetworkAssigned = network;
  network->assignToNode(this);
}

void Node::addToNetwork(Network* network)
{
  mNetworks.push_back(network);
}

void Node::setValues(double* values)
{
  for(int i=0; i<bufferSize; ++i)
  {
    mValues[i] = values[i];
  }
}

void Node::setCurrentState(state_type state)
{
  mValues[getIndexT(t)] = state[0];
}

void Node::getValues(double* values) const
{
  for(int i=0; i<bufferSize; ++i)
  {
    values[i] = mValues[i];
  }
}

double Node::getCurrentState()
{
  return mValues[getIndexT(t)];
}

double Node::getPreviousState()
{
  return mValues[getIndexTMinusOne(t)];
}

void Node::setChangeByUpwardInfluence(int index, double value)
{
  mChangeByUpwardInfluence[index] = value;
}

void Node::setChangeByDownwardInfluence(int index, double value)
{
  mChangeByDownwardInfluence[index] = value;
}

double Node::getChangeByUpwardInfluence(int index)
{
  return mChangeByUpwardInfluence[index];
}

double Node::getChangeByDownwardInfluence(int index)
{
  return mChangeByDownwardInfluence[index];
}

void Node::step(SimulationParameterContainer *parameters)
{
  std::vector<Network*> networks = getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    DynamicalEquation* dynamicalEquation = (*itNet)->getNodeDynamicalEquation(this->getId());
    stepODE(dynamicalEquation, parameters);
  }

  mUpwardInfluence->calculateUpwardInfluence();
  mDownwardInfluence->calculateDownwardInfluence();
  mVectorFieldReconfiguration->calculateVectorFieldReconfiguration(parameters->geneticParameters);
}

void Node::stepODE(DynamicalEquation* dynamicalEquation, SimulationParameterContainer *parameters)
{
  state_type x = {getPreviousState()};

  OdeWrapper wrapper(dynamicalEquation);
  integrate(wrapper, x, 0.0, parameters->odeTime, parameters->odeStepSize);  
  setCurrentState(x);
}

void Node::stepOdeAtState(DynamicalEquation* dynamicalEquation, std::vector<IdValuePair> &startingState, std::vector<IdValuePair> &finalState)
{
  state_type x = {getValueForId(startingState, getId())};
  OdeWrapperAtState wrapper(dynamicalEquation, &startingState);
  integrate(wrapper, x, 0.0, 1.0, 0.1);
  setValueForId(finalState, getId(), x[0]);
}

void Node::setUpwardInfluence()
{
  mUpwardInfluence = new UpwardInfluenceImpl(this);
}

void Node::setDownwardInfluence()
{
  mDownwardInfluence = new DownwardInfluenceImpl(this);
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

std::ostream& operator<<(std::ostream &os, const Node &node)
{
  os<<"Node "<<node.getId()<<std::endl;

  //printing state
  double* tmpBuffer = new double[bufferSize];
  node.getValues(tmpBuffer);
  for(int i=0; i<bufferSize; ++i)
  {
    os<<" "<<tmpBuffer[i];
  }
  os<<std::endl;
  delete [] tmpBuffer;

  //printing equation
  std::vector<Network*> networks = node.getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    os<<"  "<<(*itNet)->getNodeDynamicalEquationString(node.getId())<<std::endl;
  }

  return os;
}
