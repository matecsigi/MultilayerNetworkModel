#include "NetworkPopulationElement.hh"
#include "VectorFieldSchemes.hh"

NetworkPopulationElement::NetworkPopulationElement(Network* network, VectorField* targetVectorField)
{
  mTargetVectorField = targetVectorField;
  setNetwork(network);
}

NetworkPopulationElement::~NetworkPopulationElement()
{
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

void NetworkPopulationElement::updateFitness()
{
  VectorField* currentVectorField = new VectorField();
  std::vector<IdValuePair> currentState = mNetwork->getCurrentState();
  std::vector<Node*> nodes = mNetwork->getNodes();
  gridAroundPointScheme(currentVectorField, mNetwork, currentState);
  double distance = mTargetVectorField->getDistanceFrom(currentVectorField);

  double fitness = 100.0/distance;
  setFitness(fitness);

  delete currentVectorField;
}
