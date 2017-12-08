#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "IStructureGenerator.hh"
#include "StructureGeneratorBarabasiImpl.hh"
#include "IDynamicalEquationGenerator.hh"
#include "DynamicalEquationGeneratorSimpleImpl.hh"
#include "IInitialConditionGenerator.hh"
#include "InitialConditionGeneratorImpl.hh"

int main()
{
  std::cout<<"Starting generation"<<std::endl;

  const char *filename="sources/projects/BarabasiMultilayer/generated/structure.json";
  const char *filenameInitialCond="sources/projects/BarabasiMultilayer/generated/InitialConditions.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorBarabasiImpl(multilayerNetwork);
  structureGenerator->generateStructure();

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  multilayerNetwork->save(filename);

  IInitialConditionGenerator* initialConditionGenerator = new InitialConditionGeneratorImpl(multilayerNetwork);
  initialConditionGenerator->generateInitialCondition();
  multilayerNetwork->saveState(filenameInitialCond);

  cout<<*multilayerNetwork<<endl;

  delete multilayerNetwork;

  return 0;
}
