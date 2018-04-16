#ifndef VECTORFIELDTRANSFORMIMPLEMENTATIONS
#define VECTORFIELDTRANSFORMIMPLEMENTATIONS

#include "Node.hh"
#include "VectorField.hh"

void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, Node* node, SimulationParameterContainer *parameters);

void adjustVectorField(VectorField* adjustedVectorField, VectorField* beginVectorField, VectorField* endVectorField, SimulationParameterContainer *parameters);

#endif
