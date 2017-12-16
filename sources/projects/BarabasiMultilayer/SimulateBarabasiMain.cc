#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "GlobalVariables.hh"
#include <ctime>
#include <chrono>

int main()
{
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  std::cout<<"Starting simulation"<<std::endl;

  int simulationTime = 10;

  const char *filename="sources/projects/BarabasiMultilayer/generated/structure.json";
  std::string filenameInitialCond = "sources/projects/BarabasiMultilayer/generated/InitialConditions.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  multilayerNetwork->load(filename);
  multilayerNetwork->loadState(filenameInitialCond.c_str());

  for(t=0; t<simulationTime; ++t)
  {
    std::cout<<"t="<<t<<std::endl;
    multilayerNetwork->step();

    if((t % (bufferSize-2)) == (bufferSize-2-1))
    {
      std::cout<<"Changing t="<<t<<std::endl;
      multilayerNetwork->save();
      multilayerNetwork->saveState();
      multilayerNetwork->shiftBuffers();
    }
  }

  // std::cout<<*multilayerNetwork<<std::endl;

  delete multilayerNetwork;

  std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;

  return 0;
}
