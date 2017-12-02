#include "GlobalVariables.hh"

int bufferSize = 20;
int initialConditionsSize = 2;
int t = 0;

//ODE parameters
double odeTime = 1;
double odeStepSize = 0.1;

//Genetic algorithm parameters
int initialPopulationSize = 20;
int numberOfGenerations = 1;
double mutationRatio = 0.3;
double crossoverRatio = 0.3;
double deathRatio = 0.3;
