#ifndef VECTORFIELDSCHEMES_HH
#define VECTORFIELDSCHEMES_HH

#include <functional>
#include "NetworkUtilityFunctions.hh"

class Network;
class VectorField;

void gridAroundPointScheme1(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates);

void gridAroundPointScheme2(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates, std::function<std::vector<IdValuePair> (Network*, std::vector<IdValuePair>&)> getDirectionAtState = getIsolatedDirectionAtState);

#endif
