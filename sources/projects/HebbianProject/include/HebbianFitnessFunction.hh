#ifndef HEBBIANFITNESSFUNCTION_HH
#define HEBBIANFITNESSFUNCTION_HH

#include "NetworkPopulationElement.hh"
#include "MultilayerNetwork.hh"
#include "HebbianParameterContainer.hh"

double hebbianFitnessFunction(Network* networkPopulationElement, HebbianParameterContainer *hebbianParameters);
void generateMultilayerNetworkForHebbianFitness(MultilayerNetwork* multilayerNetwork, Network* network, HebbianParameterContainer *hebbianParameters);

#endif
