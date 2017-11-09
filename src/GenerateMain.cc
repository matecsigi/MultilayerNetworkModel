#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"

using namespace std;

int main(void)
{
  cout<<"Start generation"<<endl;

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* generator = new StructureGeneratorImpl(multilayerNetwork);
  generator->generateStructure();

  const char *filename="generated/MultilayerNetworkStructure-1.json";
  multilayerNetwork->save(filename);

  MultilayerNetwork* multilayerNetwork2 = new MultilayerNetwork;
  multilayerNetwork2->load(filename);

  std::cout<<*multilayerNetwork2<<std::endl;

  delete multilayerNetwork;

  return 0;
}
