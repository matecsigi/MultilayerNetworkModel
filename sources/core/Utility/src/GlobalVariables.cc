#include "GlobalVariables.hh"

int bufferSize = 10;
int initialConditionsSize = 2;
int t = 0;

//ODE parameters
double odeTime = 1;
double odeStepSize = 0.1;

//Genetic algorithm parameters
int initialPopulationSize = 10;
int numberOfGenerations = 2;
double mutationRatio = 0.4;
double crossoverRatio = 0.4;
double deathRatio = 0.2;