#ifndef UTILITYFUNCTIONS_HH
#define UTILITIFUNCTIONS_HH

#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "IdValuePair.hh"
#include "VectorField.hh"

int getIndexT(int t);
int getIndexTMinusOne(int t);
int getIndexTMinusTwo(int t);

void setIds(Layer* layer, Network* network);
int getMaxNetworkId(MultilayerNetwork *multilayerNetwork);
int getMaxNodeId(MultilayerNetwork *multilayerNetwork);

double getValueForId(std::vector<IdValuePair> &pairVector, int id);
void setValueForId(std::vector<IdValuePair> &pairVector, int id, double value);

VectorFieldPoint* findCorrespondingPoint(VectorFieldPoint* point, VectorField* vectorField);

void loadNetworkFromJSON(Network* network, std::string filename, int& nodeIdCounter);
void saveNetworkToJSON(Network* network, std::string filename);

void loadPopulation(Network* network, int control, std::string folderName);

void printDirection(std::vector<IdValuePair> direction);

#endif