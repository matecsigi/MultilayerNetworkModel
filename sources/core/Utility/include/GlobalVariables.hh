#ifndef GLOBALVARIABLES_HH
#define GLOBALVARIABLES_HH

#include <vector>

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
extern double elitRatio;

//Network modification parameters
/**
   Contains the ratios of different network/equation modification types.
 */
extern std::vector<double> vectorReconfModTypeProbabilities;
extern std::vector<double> hebbianModTypeProbabilities;

#endif
