#ifndef HEBBIANFITNESSFUNCTION_HH
#define HEBBIANFITNESSFUNCTION_HH

#include "NetworkPopulationElement.hh"
#include "MultilayerNetwork.hh"

double hebbianFitnessFunction(NetworkPopulationElement* networkPopulationElement);
void generateMultilayerNetworkForHebbianFitness(MultilayerNetwork* multilayerNetwork, Network* network);

#endif
