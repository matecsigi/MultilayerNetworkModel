#include <iostream>
#include "Node.hh"
#include "Layer.hh"
#include "MultilayerNetwork.hh"

int main(void)
{
  std::cout<<"Hello World!"<<std::endl;

  MultilayerNetwork network;
  for(int i;i<3;i++)
  {
    network.addLayer(i);
  }

  std::vector<Layer*> layers = network.getLayers();
  for(std::vector<Layer*>::iterator it=layers.begin(); it != layers.end(); ++it)
  {
    for(int i=0; i<10; i++)
    {
      (*it)->addNode(i);
    }
  }

  return 0;
}
