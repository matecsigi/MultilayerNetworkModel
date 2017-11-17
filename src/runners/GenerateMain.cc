#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"
#include "IInitialConditionGenerator.hh"
#include "InitialConditionGeneratorImpl.hh"

using namespace std;

int main(void)
{
  cout<<"Start generation"<<endl;

  const char *filename="generated/MultilayerNetworkStructure-1.json";
  const char *filenameInitialCond="generated/InitialCondition-1.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure();
  multilayerNetwork->save(filename);

  IInitialConditionGenerator* initialConditionGenerator = new InitialConditionGeneratorImpl(multilayerNetwork);
  initialConditionGenerator->generateInitialCondition();
  multilayerNetwork->saveState(filenameInitialCond);

  delete multilayerNetwork;

  return 0;
}
