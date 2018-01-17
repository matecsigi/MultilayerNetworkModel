#ifndef NETWORKINITIALCONDITIONGENERATORS_HH
#define NETWORKINITIALCONDITIONGENERATORS_HH

#include "Network.hh"

void oneNetworkInitialConditions(Network* network);
void randomNetworkInitialConditions(Network* network, double min = 0, double max = 10);

#endif
