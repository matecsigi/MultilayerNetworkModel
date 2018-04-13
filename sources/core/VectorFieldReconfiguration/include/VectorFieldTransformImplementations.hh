#ifndef VECTORFIELDTRANSFORMIMPLEMENTATIONS
#define VECTORFIELDTRANSFORMIMPLEMENTATIONS

#include "Node.hh"
#include "VectorField.hh"

void calculateTargetVectorField(VectorField* targetVectorField, VectorField* currentVectorField, Node* node);

/**
   Takes the direction of the assigned node in the higher network and each direction
   value in the lower network is multiplied by it to get the change in the direction change 
   to that node.
   It matters which higher node the lower network is attached to.
   The directions in other higher network nodes doesn't matter.
 */
void calculateTargetVectorFieldOld(VectorField* targetVectorField, VectorField* currentVectorField, Node* node);

/**
   Calculates the sum of the directions in the higher network(s) and each direction
   value in the lower network is multiplied by it to get the change in the direction change 
   to that node.
   It doesn't matter which higher node the lower network is attached to, because this sum value 
   doesn't take that into account.
   The diretions in other higher nodes matter as they contribute to the sum.
 */
void sumVectorFieldTransform(VectorField* targetVectorField, VectorField* currentVectorField, Node* node);

/**
   Calculates the sum of the directions in the higher network(s)  but the node to which the 
   lower network is attached to recieves extra weight.
   Both the node to which the lower level network is attached and the directions in the other
   nodes matter.
 */
void weightNodeVectorFieldTransform(VectorField* targetVectorField, VectorField* currentVectorField, Node* node);

#endif
