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
  hebbianParameters->transientTime = 10;
  hebbianParameters->runTime = 10;
  hebbianParameters->numberOfIterations = 2;

  int iterations = 5;
  int transientCounter = 10;

  while(transientCounter < 20)
  {
    std::cout<<"--transient="<<transientCounter<<std::endl;
    hebbianParameters->transientTime = transientCounter;

    double sum1 = 0;
    double fitness1;
    double min1 = 1000;
    double max1 = 0;

    for(int i=0; i<iterations; ++i)
    {
      fitness1 = hebbianFitnessFunction(network1, hebbianParameters);
      sum1 += fitness1;
      if(fitness1 < min1)
      {
	min1 = fitness1;
      }
      if(fitness1 > max1)
      {
	max1 = fitness1;
      }
      std::cout<<"  fitness="<<fitness1<<std::endl;
    }
    std::cout<<"    min="<<min1<<std::endl;
    std::cout<<"    max="<<max1<<std::endl;
    std::cout<<"    avg="<<sum1/iterations<<std::endl;

    transientCounter += 20;
  }
  return 0;
}
