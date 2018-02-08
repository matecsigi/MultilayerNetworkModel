#include <iostream>
#include "HebbianFitnessFunction.hh"

int main()
{
  std::cout<<"Hello Analyzis"<<std::endl;

  int tmp = 1;
  Network* network1 = new Network;
  std::string filename1 = "sources/projects/Analyzis/network1.json";
  loadNetworkFromJSON(network1, filename1, tmp);
  Network* network2 = new Network;
  std::string filename2 = "sources/projects/Analyzis/network2.json";
  loadNetworkFromJSON(network2, filename2, tmp);


  HebbianParameterContainer *hebbianParameters = new HebbianParameterContainer;
  hebbianParameters->transientTime = 20;
  hebbianParameters->runTime = 20;

  int iterations = 20;
  double sum1 = 0;
  double sum2 = 0; 

  for(int i=0; i<iterations; ++i)
  {
    std::cout<<"Network 1"<<std::endl;
    sum1 += hebbianFitnessFunction(network1, hebbianParameters);
    std::cout<<"--AVG1="<<sum1/(double)(i+1)<<std::endl;

    std::cout<<"Network 2"<<std::endl;
    sum2 += hebbianFitnessFunction(network2, hebbianParameters);
    std::cout<<"--AVG2="<<sum2/(double)(i+1)<<std::endl;
  }

  return 0;
}
