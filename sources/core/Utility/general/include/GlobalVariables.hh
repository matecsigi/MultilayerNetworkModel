#ifndef GLOBALVARIABLES_HH
#define GLOBALVARIABLES_HH

#include <vector>

extern int numberOfCores;

extern int bufferSize;
extern int initialConditionsSize;

//Network modification parameters
/**
   Contains the ratios of different network/equation modification types.
 */
extern std::vector<double> vectorReconfModTypeProbabilities;
extern std::vector<double> hebbianModTypeProbabilities;

#endif
