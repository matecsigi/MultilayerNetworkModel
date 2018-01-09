#ifndef NETWORKPOPULATIONELEMENT_HH
#define NETWORKPOPULATIONELEMENT_HH

#include "Network.hh"
#include "VectorField.hh"

class NetworkPopulationElement
{
public:
  NetworkPopulationElement(Network* network, VectorField* targetVectorField, double (*fitnessFunction)(NetworkPopulationElement*) = NULL);
  ~NetworkPopulationElement();

  void setNetwork(Network* network);
  void setGeneration(int generation);
  void setFitness(double fitness);
  void setRank(int rank);

  Network* getNetwork();
  int getGeneration();
  double getFitness();
  int getRank();

  void updateFitness();
  friend double vectorReconfFitnessFunction(NetworkPopulationElement *networkPopulationElement);

private:
  VectorField* mTargetVectorField;
  Network* mNetwork;
  int mGeneration;
  double mFitness;
  double (*mFitnessFunction)(NetworkPopulationElement*);
  /**
     The rank of the network in the population based on fitness.
     The element with rank 1 has thi highest fitness value.
   */
  double mRank;
};

#endif
