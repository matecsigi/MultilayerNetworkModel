#ifndef VECTORFIELDSCHEMES_HH
#define VECTORFIELDSCHEMES_HH

#include "VectorField.hh"
#include "Network.hh"

void gridAroundPointScheme(VectorField* vectorField, Network* network, std::map<int, double> basePointCoordinates);

#endif
