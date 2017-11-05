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

  IStructureGenerator* generator = new StructureGeneratorImpl();
  generator->generateStructure(multilayerNetwork);

  delete multilayerNetwork;

  return 0;
}
