#include "GlobalVariables.hh"

int numberOfCores = 1;

int bufferSize = 10;
int initialConditionsSize = 2;
int t = 0;

//ODE parameters
double odeTime = 1;
double odeStepSize = 0.1;

//Genetic algorithm parameters
int initialPopulationSize = 10; //250
int numberOfGenerations = 5; //50
double mutationRatio = 0.15;
double crossoverRatio = 0.15;
double deathRatio = 0.3;

/**
   The highest rankded elements are not deleted.
   This strategy is called elitism.
 */
double elitRatio = 0.5;

//Network modification parameters
std::vector<double> vectorReconfModTypeProbabilities
{
  1, //ADD_EDGE
  1, //REMOVE_EDGE
  3, //ADD_TO_OUTER_BLOCK
  3, //REMOVE_FROM_OUTER_BLOCK
  100, //CHANGE_CONSTANT
  20, //CHANGE_PLUS_TO_MULTIPLY
  20 //CHANGE_MULTIPLY_TO_PLUS
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
