#ifndef VECTORFIELDSCHEMES_HH
#define VECTORFIELDSCHEMES_HH

#include "VectorField.hh"
#include "Network.hh"

void gridAroundPointScheme1(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates);

void gridAroundPointScheme2(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates);

#endif
