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

//Structure building

void setIds(Layer* layer, Network* network);
int getMaxNetworkId(MultilayerNetwork *multilayerNetwork);
int getMaxNodeId(MultilayerNetwork *multilayerNetwork);

bool containsDuplicate(std::vector<int> &vectorToCheck);

//IdValuePair

double getValueForId(std::vector<IdValuePair> &pairVector, int id);
void setValueForId(std::vector<IdValuePair> &pairVector, int id, double value);

std::vector<int> getIds(std::vector<IdValuePair> &pairVector);
std::vector<double> getValues(std::vector<IdValuePair> &pairVector);

VectorFieldPoint* findCorrespondingPoint(VectorFieldPoint* point, VectorField* vectorField);

void loadNetworkFromJSON(Network* network, std::string filename, int& nodeIdCounter);
void saveNetworkToJSON(Network* network, std::string filename);

void loadPopulation(Network* network, int control, std::string folderName);

void createDirectory(std::string dirName);

void printDirection(std::vector<IdValuePair> direction);

#endif
