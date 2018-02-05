#ifndef NETWORKPOPULATIONELEMENT_HH
#define NETWORKPOPULATIONELEMENT_HH

#include "Network.hh"
#include "VectorField.hh"
#include <functional>

class NetworkPopulationElement
{
public:
  NetworkPopulationElement(Network* network, VectorField* targetVectorField, std::function<double (Network*)> fitnessFunction = nullptr);
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
  friend double vectorReconfFitnessFunction(Network* network, VectorField *targetVectorField);

private:
  VectorField* mTargetVectorField;
  Network* mNetwork;
  int mGeneration;
  double mFitness;
  std::function<double (Network*)> mFitnessFunction;
  /**
     The rank of the network in the population based on fitness.
     The element with rank 1 has thi highest fitness value.
   */
  double mRank;
};

#endif
