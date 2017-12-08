#ifndef UTILITYFUNCTIONS_HH
#define UTILITIFUNCTIONS_HH

#include "Network.hh"

int getIndexT(int t);
int getIndexTMinusOne(int t);
int getIndexTMinusTwo(int t);

void loadNetworkFromJSON(Network* network, std::string filename, int& nodeIdCounter);

#endif
