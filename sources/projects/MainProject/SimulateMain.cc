#include <iostream>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "GlobalVariables.hh"
#include <ctime>
#include <chrono>
#include "Trace.hh"

int main(void)
{
  traceRun("Hello Trace!");

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  std::cout<<"Start simulation"<<std::endl;

  int simulationTime = 100;

  const char *filename="generated/multilayerStructureInit.json";
  std::string filenameInitialCond = "generated/InitialCondition-1.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  multilayerNetwork->load(filename);
  multilayerNetwork->loadState(filenameInitialCond.c_str());
  multilayerNetwork->iterate(simulationTime);

  // std::cout<<*multilayerNetwork<<std::endl;

  delete multilayerNetwork;

  std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;

  return 0;
}
