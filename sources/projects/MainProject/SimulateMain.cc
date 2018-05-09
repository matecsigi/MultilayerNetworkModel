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
  std::cout<<"TRACELEVEL="<<TRACELEVEL<<std::endl;
  traceRun("Start Simulation\n");

  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  int simulationTime = 10;

  const char *filename="generated/multilayerStructureInit.json";
  std::string filenameInitialCond = "generated/InitialCondition-1.bin";

  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
  multilayerNetwork->load(filename);
  multilayerNetwork->loadState(filenameInitialCond.c_str());

  SimulationParameterContainer *parameters = new SimulationParameterContainer;
  parameters->printTrace = true;

  multilayerNetwork->print();
  multilayerNetwork->iterate(simulationTime, parameters);

  delete multilayerNetwork;

  std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
  std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;

  return 0;
}
