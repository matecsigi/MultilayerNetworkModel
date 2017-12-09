#include "StructureGeneratorBarabasiImpl.hh"
#include "UtilityFunctions.hh"
// #include <unistd.h>
#include <stdlib.h>

StructureGeneratorBarabasiImpl::StructureGeneratorBarabasiImpl(MultilayerNetwork* multilayerNetwork)
{
  mMultilayerNetwork = multilayerNetwork;
  mNumberOfNodesOnTopLayer = 30;
  mNumberOfNodesInANetwork = 30;
}

void StructureGeneratorBarabasiImpl::generateStructure()
{
  addLayers();
  addNetworks();
  generateNetworks();
  assignNetworksToNodes();
}

void StructureGeneratorBarabasiImpl::addLayers(void)
{
  for(int i=1;i<4;i++)
  {
    mMultilayerNetwork->addLayer(i);
  }
}

void StructureGeneratorBarabasiImpl::addNetworks(void)
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  int networkIdCounter = 0;
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    int layerIndex = std::distance(layers.begin(), itLay);
    if(layerIndex == 0)
    {
      (*itLay)->addNetwork(networkIdCounter+1);
      ++networkIdCounter;
      continue;
    }
    for(int i=0; i<pow(mNumberOfNodesInANetwork, layerIndex-1)*mNumberOfNodesOnTopLayer; ++i)
    {
      (*itLay)->addNetwork(networkIdCounter+1);
      ++networkIdCounter;
    }
  }
}

void StructureGeneratorBarabasiImpl::generateNetworks()
{
  int nodeIdCounter = 1;
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  for(std::vector<Layer*>::iterator itLay=layers.begin(); itLay != layers.end(); ++itLay)
  {
    int layerId = (*itLay)->getId();
    std::vector<Network*> networks = (*itLay)->getNetworks();
    for(std::vector<Network*>::iterator itNet=networks.begin(); itNet != networks.end(); ++itNet)
    {
      int networkId =(*itNet)->getId();
      std::string filename = pythonBarabasiGenerator(layerId, networkId);
      std::cout<<"layer="<<layerId<<" network="<<networkId<<std::endl;
      loadNetworkFromJSON((*itNet), filename, nodeIdCounter);
    }
  }
}

std::string StructureGeneratorBarabasiImpl::pythonBarabasiGenerator(int layerId, int networkId)
{
  std::string filename = "/home/mate/Documents/Programming/Research/MultilayerNetworkModel/MultilayerNetworkModel/sources/projects/BarabasiMultilayer/generated/barabasi_";
  filename.append(std::to_string(layerId));
  filename.append("_");
  filename.append(std::to_string(networkId));
  filename.append(".json");

  // execl("/usr/bin/python2.7", "barabasiGenerator.py", mNumberOfNodesInANetwork, filename.c_str(),  (char*)NULL);

  std::string command = "python /home/mate/Documents/Programming/Research/MultilayerNetworkModel/MultilayerNetworkModel/sources/projects/BarabasiMultilayer/barabasiGenerator.py ";
  command.append(std::to_string(mNumberOfNodesInANetwork));
  command.append(" ");
  command.append(filename);

  system(command.c_str());

  return filename;
}

void StructureGeneratorBarabasiImpl::assignNetworksToNodes()
{
  std::vector<Layer*> layers = mMultilayerNetwork->getLayers();
  for(unsigned i=0; i<layers.size()-1; ++i)
  {
    std::vector<Network*> networksUp = layers[i]->getNetworks();
    std::vector<Network*> networksDown = layers[i+1]->getNetworks();
    int nodeCounter = 0;
    for(std::vector<Network*>::iterator itNet=networksUp.begin(); itNet != networksUp.end(); ++itNet)
    {
      std::vector<Node*> nodesInNetwork = (*itNet)->getNodes();
      for(std::vector<Node*>::iterator itNode=nodesInNetwork.begin(); itNode != nodesInNetwork.end(); ++itNode)
      {
	(*itNode)->setNetworkAssigned(networksDown[nodeCounter]);
  	 ++nodeCounter;
      }
    }
  }
}
