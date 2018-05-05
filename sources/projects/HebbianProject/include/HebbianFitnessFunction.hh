#ifndef HEBBIANFITNESSFUNCTION_HH
#define HEBBIANFITNESSFUNCTION_HH

#include "HebbianParameterContainer.hh"

class MultilayerNetwork;
class Network;
class NetworkPopulationElement;

double hebbianFitnessFunction(Network* networkPopulationElement, HebbianParameterContainer *hebbianParameters);
void generateMultilayerNetworkForHebbianFitness(MultilayerNetwork* multilayerNetwork, Network* network, HebbianParameterContainer *hebbianParameters);

#endif
