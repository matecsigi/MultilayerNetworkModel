#include "Node.hh"
#include <iostream>
#include "UpwardInfluenceImpl.hh"
#include "DownwardInfluenceImpl.hh"
#include "VectorFieldReconfigurationImpl.hh"
#include "OdeWrapper.hh"
#include "OdeWrapperAtState.hh"
#include "Trace.hh"
#include <boost/numeric/odeint.hpp>
#include <iomanip>
#include <sstream>

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
  mMultilayerNetwork = NULL;
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
  mMultilayerNetwork = NULL;
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

int Node::getTime()
{
  if(mMultilayerNetwork != NULL)
  {
    return mMultilayerNetwork->getTime();
  }
  Network *network = mNetworks[0];
  return network->getTime();
}

int Node::getId(void) const
{
  return mNodeId;
}

void Node::setId(int id)
{
  mNodeId = id;
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
  network->setNodeAssigned(this);
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

void Node::setCurrentState(double state)
{
  mValues[getIndexT(getTime())] = state;
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
  return mValues[getIndexT(getTime())];
}

double Node::getPreviousState()
{
  return mValues[getIndexTMinusOne(getTime())];
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

  mUpwardInfluence->calculateUpwardInfluence(parameters->upwardInfluenceMultiplier);
  mDownwardInfluence->calculateDownwardInfluence(parameters->downwardInfluenceMultiplier);
  if(parameters->runVectorFieldReconfiguration)
  {
    mVectorFieldReconfiguration->calculateVectorFieldReconfiguration(parameters);
  }
}

void Node::stepODE(DynamicalEquation* dynamicalEquation, SimulationParameterContainer *parameters)
{
  state_type x = {getPreviousState()};

  OdeWrapper wrapper(dynamicalEquation);
  integrate(wrapper, x, 0.0, parameters->odeTime, parameters->odeStepSize);  
  setCurrentState(x[0]);
}

void Node::stepOdeAtState(DynamicalEquation* dynamicalEquation, std::vector<IdValuePair> &startingState, std::vector<IdValuePair> &finalState)
{
  state_type x = {getValueForId(startingState, getId())};
  OdeWrapperAtState wrapper(dynamicalEquation, &startingState);
  integrate(wrapper, x, 0.0, 1.0, 0.1);
  setValueForId(finalState, getId(), x[0]);
}

void Node::setMultilayerNetwork(MultilayerNetwork *multilayerNetwork)
{
  mMultilayerNetwork = multilayerNetwork;
}

void Node::print()
{
  traceDebug("Node "+std::to_string(mNodeId)+"\n");
  std::vector<Network*> networks = getNetworks();
  for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
  {
    if(mNetworkAssigned != NULL)
    {
      traceDebug("        ");
      traceDebug("NetworkAssigned "+std::to_string(mNetworkAssigned->getId())+"\n");
    }
    else
    {
      traceDebug("        ");
      traceDebug("NetworkAssigned NULL\n");
    }

    traceDebug("        ");
    traceDebug("Equation "+(*itNet)->getNodeDynamicalEquationString(mNodeId)+"\n");
    traceDetailed("        ");
    traceDetailed("State ");
    for(int i=0; i<bufferSize; ++i)
    {
      std::stringstream streamValue;
      streamValue<<std::fixed<<std::setprecision(2)<<mValues[i];
      std::string strValue = streamValue.str();
      traceDetailed(strValue);
      traceDetailed(" ");
    }
    traceDetailed("\n");
  }
}
