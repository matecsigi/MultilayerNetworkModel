#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "GlobalVariables.hh"

using namespace std;

int main(void)
{
  cout<<"Start simulation"<<endl;
  cout<<"bufferSize="<<bufferSize<<endl;

  int simulationTime = 5;

  const char *filename="generated/MultilayerNetworkStructure-1.json";
  const char *filenameInitialCond="generated/InitialCondition-1.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  multilayerNetwork->load(filename);
  multilayerNetwork->loadState(filenameInitialCond);

  for(t=0; t<simulationTime; ++t)
  {
    multilayerNetwork->step();
  }

  std::cout<<*multilayerNetwork<<std::endl;

  delete multilayerNetwork;

  return 0;
}
