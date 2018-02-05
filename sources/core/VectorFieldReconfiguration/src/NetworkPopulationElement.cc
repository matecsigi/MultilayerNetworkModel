#include "NetworkPopulationElement.hh"
#include "VectorFieldSchemes.hh"

using namespace std::placeholders;

double vectorReconfFitnessFunction(Network* network, VectorField* targetVectorField)
{
  VectorField* currentVectorField = new VectorField();
  std::vector<IdValuePair> currentState = network->getCurrentState();
  gridAroundPointScheme2(currentVectorField, network, currentState);
  double distance = targetVectorField->getDistanceFrom(currentVectorField);

  double fitness = 100.0/distance;
  delete currentVectorField;

  return fitness;
}

NetworkPopulationElement::NetworkPopulationElement(Network* network, VectorField* targetVectorField, std::function<double (Network*)> fitnessFunction)
{
  mTargetVectorField = targetVectorField;
  if(fitnessFunction == nullptr)
  {
    mFitnessFunction = std::bind(vectorReconfFitnessFunction, _1, mTargetVectorField);
  }
  else
  {
    mFitnessFunction = fitnessFunction;
  }
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
  double fitness = mFitnessFunction(mNetwork);
  setFitness(fitness);
}
