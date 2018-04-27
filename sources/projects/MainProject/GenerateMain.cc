#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"
#include "IDynamicalEquationGenerator.hh"
#include "DynamicalEquationGeneratorSimpleImpl.hh"
#include "IInitialConditionGenerator.hh"
#include "InitialConditionGeneratorImpl.hh"
#include "Trace.hh"

int main(void)
{
  std::cout<<"Start generation"<<std::endl;

  const char *filename="generated/multilayerStructureInit.json";
  const char *filenameInitialCond="generated/InitialCondition-1.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure(6, 3);

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  multilayerNetwork->save(filename);

  IInitialConditionGenerator* initialConditionGenerator = new InitialConditionGeneratorImpl(multilayerNetwork);
  initialConditionGenerator->generateInitialCondition();
  multilayerNetwork->saveState(filenameInitialCond);

  std::cout<<"Generated MultilayerNetwork"<<std::endl;
  multilayerNetwork->print();

  delete multilayerNetwork;

  return 0;
}
