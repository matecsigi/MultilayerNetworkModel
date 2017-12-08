#ifndef GLOBALVARIABLES_HH
#define GLOBALVARIABLES_HH

extern int numberOfCores;

extern int bufferSize;
extern int initialConditionsSize;
extern int t;

//ODE parameters
extern double odeTime;
extern double odeStepSize;

//Genetic algorithm parameters
extern int initialPopulationSize;
extern int numberOfGenerations;
extern double mutationRatio;
extern double crossoverRatio;
extern double deathRatio;

#endif
