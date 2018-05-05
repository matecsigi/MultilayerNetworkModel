#ifndef VECTORFIELDTRANSFORMIMPLEMENTATIONS
#define VECTORFIELDTRANSFORMIMPLEMENTATIONS

#include "SimulationParameterContainer.hh"

class Node;
class VectorField;

void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, Node* node, SimulationParameterContainer *parameters);

void adjustVectorField(VectorField* adjustedVectorField, VectorField* beginVectorField, VectorField* endVectorField, SimulationParameterContainer *parameters);

#endif
