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

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure();
  const char *filename="generated/MultilayerNetworkStructure-1.json";
  multilayerNetwork->save(filename);

  IInitialConditionGenerator* initialConditionGenerator = new InitialConditionGeneratorImpl(multilayerNetwork);
  initialConditionGenerator->generateInitialCondition();
  const char *filenameInitialCond="generated/InitialCondition-1.bin";
  multilayerNetwork->saveState(filenameInitialCond);

  //std::cout<<*multilayerNetwork<<std::endl;

  delete multilayerNetwork;

  return 0;
}
