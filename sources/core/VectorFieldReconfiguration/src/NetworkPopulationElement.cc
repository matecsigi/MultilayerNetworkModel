#include "NetworkPopulationElement.hh"
#include "VectorFieldSchemes.hh"

double vectorReconfFitnessFunction(NetworkPopulationElement *networkPopulationElement)
{
  VectorField* currentVectorField = new VectorField();
  std::vector<IdValuePair> currentState = networkPopulationElement->mNetwork->getCurrentState();
  gridAroundPointScheme2(currentVectorField, networkPopulationElement->mNetwork, currentState);
  double distance = networkPopulationElement->mTargetVectorField->getDistanceFrom(currentVectorField);

  double fitness = 100.0/distance;
  delete currentVectorField;

  return fitness;
}

NetworkPopulationElement::NetworkPopulationElement(Network* network, VectorField* targetVectorField, std::function<double (NetworkPopulationElement*)> fitnessFunction)
{
  if(fitnessFunction == nullptr)
  {
    mFitnessFunction = &vectorReconfFitnessFunction;
  }
  else
  {
    mFitnessFunction = fitnessFunction;
  }
  mTargetVectorField = targetVectorField;
  setNetwork(network);
}

NetworkPopulationElement::~NetworkPopulationElement()
{
  delete mNetwork;
}

void NetworkPopulationElement::setNetwork(Network* network)
{
  mNetwork = network;
  updateFitness();
}

void NetworkPopulationElement::setGeneration(int generation)
{
  mGeneration = generation;
}

void NetworkPopulationElement::setFitness(double fitness)
{
  mFitness = fitness;
}

void NetworkPopulationElement::setRank(int rank)
{
  mRank = rank;
}

Network* NetworkPopulationElement::getNetwork()
{
  return mNetwork;
}

int NetworkPopulationElement::getGeneration()
{
  return mGeneration;
}

double NetworkPopulationElement::getFitness()
{
  return mFitness;
}

int NetworkPopulationElement::getRank()
{
  return mRank;
}

void NetworkPopulationElement::updateFitness()
{
  double fitness = mFitnessFunction(this);
  setFitness(fitness);
}
