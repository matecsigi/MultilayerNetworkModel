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

  int simulationTime = 30;

  const char *filename="generated/multilayerStructure_0.json";
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
      cout<<"Changing t="<<t<<endl;
      multilayerNetwork->save();
      multilayerNetwork->saveState();
      multilayerNetwork->shiftBuffers();
    }
  }

  // std::cout<<*multilayerNetwork<<std::endl;

  delete multilayerNetwork;

  return 0;
}
