#include "GlobalVariables.hh"

int numberOfCores = 1;

int bufferSize = 100;
int initialConditionsSize = 2;

//Network modification parameters
std::vector<double> vectorReconfModTypeProbabilities
{
  1, //1 //ADD_EDGE
  1, //1  //REMOVE_EDGE
  3, // 3 //ADD_TO_OUTER_BLOCK
  3, //3 //REMOVE_FROM_OUTER_BLOCK
  100, //100 // 100//CHANGE_CONSTANT
  20, //20 //CHANGE_PLUS_TO_MULTIPLY
  20 //20 //CHANGE_MULTIPLY_TO_PLUS
};

std::vector<double> hebbianModTypeProbabilities
{
  0, //ADD_EDGE
  0, //REMOVE_EDGE
  0, //ADD_TO_OUTER_BLOCK
  0, //REMOVE_FROM_OUTER_BLOCK
  1, //CHANGE_CONSTANT
  0, //CHANGE_PLUS_TO_MULTIPLY
  0 //CHANGE_MULTIPLY_TO_PLUS
};
