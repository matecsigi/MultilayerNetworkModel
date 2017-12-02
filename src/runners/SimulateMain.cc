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

  int simulationTime = 10;

  const char *filename="generated/MultilayerNetworkStructure-1.json";
  // const char *filenameInitialCond="generated/InitialCondition-1.bin";

  std::string filenameInitialCond = "generated/InitialCondition-1.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  multilayerNetwork->load(filename);
  multilayerNetwork->loadState(filenameInitialCond.c_str());

  for(t=0; t<simulationTime; ++t)
  {
    cout<<"t="<<t<<endl;
    multilayerNetwork->step();

    if((t % (bufferSize-2)) == (bufferSize-2-1))
    {
      // std::cout<<"BEFORE SHIFT"<<std::endl;
      // std::cout<<*multilayerNetwork<<std::endl;
      cout<<"Changing t="<<t<<endl;
      multilayerNetwork->saveState();
      multilayerNetwork->shiftBuffers();
      // std::cout<<"AFTER SHIFT"<<std::endl;
      // std::cout<<*multilayerNetwork<<std::endl;
    }
  }

  // std::cout<<*multilayerNetwork<<std::endl;

  delete multilayerNetwork;

  return 0;
}
