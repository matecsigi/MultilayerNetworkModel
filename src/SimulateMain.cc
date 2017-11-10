#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"

using namespace std;

int main(void)
{
  cout<<"Start simulation"<<endl;

  const char *filename="generated/MultilayerNetworkStructure-1.json";
  const char *filenameInitialCond="generated/InitialCondition-1.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  multilayerNetwork->load(filename);
  multilayerNetwork->loadState(filenameInitialCond);
  //  std::cout<<*multilayerNetwork<<std::endl;

  delete multilayerNetwork;

  return 0;
}
