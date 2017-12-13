#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "GlobalVariables.hh"
#include <ctime>

using namespace std;

int main(void)
{
  clock_t begin = clock();

  cout<<"Start simulation"<<endl;

  int simulationTime = 6;

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

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  std::cout<<"run time="<<elapsed_secs<<std::endl;

  return 0;
}
