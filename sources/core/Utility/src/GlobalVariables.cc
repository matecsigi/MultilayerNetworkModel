#include "GlobalVariables.hh"

int numberOfCores = 4;

int bufferSize = 100;
int initialConditionsSize = 2;
int t = 0;

//ODE parameters
double odeTime = 1;
double odeStepSize = 0.1;

//Genetic algorithm parameters
int initialPopulationSize = 500;
int numberOfGenerations = 10000;
double mutationRatio = 0.15;
double crossoverRatio = 0.3;
double deathRatio = 0.3;

/**
   The highest rankded elements are not deleted.
   This strategy is called elitism.
 */
double elitRatio = 0.5;

//Network modification parameters
std::vector<double> modificationTypeProbabilities
{
  1, //ADD_EDGE
  1, //REMOVE_EDGE
  3, //ADD_TO_OUTER_BLOCK
  3, //REMOVE_FROM_OUTER_BLOCK
  100, //CHANGE_CONSTANT
  20, //CHANGE_PLUS_TO_MULTIPLY
  20 //CHANGE_MULTIPLY_TO_PLUS
};
