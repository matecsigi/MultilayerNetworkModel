#ifndef UTILITYFUNCTIONS_HH
#define UTILITYFUNCTIONS_HH

#include "IdValuePair.hh"
#include <vector>
#include <map>

class MultilayerNetwork;
class Layer;
class Network;
class VectorField;
class VectorFieldPoint;

int getIndexT(int t);
int getIndexTMinusOne(int t);
int getIndexTMinusTwo(int t);

//Structure building

void setIds(Layer* layer, Network* network, std::map<int,int>* idMap = NULL);
int getMaxNetworkId(MultilayerNetwork *multilayerNetwork);
int getMaxNodeId(MultilayerNetwork *multilayerNetwork);

bool containsDuplicate(std::vector<int> &vectorToCheck);

//IdValuePair

double getValueForId(std::vector<IdValuePair> &pairVector, int id);
void setValueForId(std::vector<IdValuePair> &pairVector, int id, double value);

std::vector<int> getIds(std::vector<IdValuePair> &pairVector);
std::vector<double> getValues(std::vector<IdValuePair> &pairVector);

double coordinateDistance(std::vector<IdValuePair>& coordinate1, std::vector<IdValuePair>& coordinate2);

void loadNetworkFromJSON(Network* network, std::string filename, int& nodeIdCounter);
void saveNetworkToJSON(Network* network, std::string filename);

void loadPopulation(Network* network, int control, std::string folderName);

void createDirectory(std::string dirName);

void printDirection(std::vector<IdValuePair> direction);

#endif
