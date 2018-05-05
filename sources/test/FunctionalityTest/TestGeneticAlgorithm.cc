#include "MultilayerNetwork.hh"
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"
#include "IDynamicalEquationGenerator.hh"
#include "DynamicalEquationGeneratorSimpleImpl.hh"
#include "IInitialConditionGenerator.hh"
#include "InitialConditionGeneratorImpl.hh"
#include "Trace.hh"
#include <iostream>

int main()
{
  traceRun("Start genetic algorithm test\n");

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure(6, 2);

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  IInitialConditionGenerator* initialConditionGenerator = new InitialConditionGeneratorImpl(multilayerNetwork);
  initialConditionGenerator->generateInitialCondition();

  SimulationParameterContainer *parameters = new SimulationParameterContainer;
  parameters->printTrace = true;
  parameters->geneticParameters->numberOfGenerations = 1000;

  multilayerNetwork->iterate(2, parameters);

  delete multilayerNetwork;

  return 0;
}
