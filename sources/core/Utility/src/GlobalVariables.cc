#include "GlobalVariables.hh"

int numberOfCores = 1;

int bufferSize = 100;
int initialConditionsSize = 2;
int t = 0;

//ODE parameters
double odeTime = 1;
double odeStepSize = 0.1;

//Genetic algorithm parameters
int initialPopulationSize = 40;
int numberOfGenerations = 10;
double mutationRatio = 0.15;
double crossoverRatio = 0.3;
double deathRatio = 0.3;
