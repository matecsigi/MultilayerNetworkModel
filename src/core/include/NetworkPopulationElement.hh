#ifndef NETWORKPOPULATIONELEMENT_HH
#define NETWORKPOPULATIONELEMENT_HH

#include "Network.hh"
#include "VectorField.hh"

class NetworkPopulationElement
{
public:
  NetworkPopulationElement(Network* network, VectorField* targetVectorField);
  ~NetworkPopulationElement();

  void setNetwork(Network* network);
  void setGeneration(int generation);
  void setFitness(double fitness);

  Network* getNetwork();
  int getGeneration();
  double getFitness();

  void updateFitness();
private:
  VectorField* mTargetVectorField;
  Network* mNetwork;
  int mGeneration;
  double mFitness;
};

#endif
