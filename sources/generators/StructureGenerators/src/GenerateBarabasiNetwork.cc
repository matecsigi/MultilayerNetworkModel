#include "GenerateBarabasiNetwork.hh"
#include "UtilityFunctions.hh"

std::string pythonBarabasiGeneratorIndependent(int numberOfNodes)
{
  // std::string filename = "/home/mate/Documents/Programming/Research/MultilayerNetworkModel/MultilayerNetworkModel/sources/projects/BarabasiMultilayer/generated/barabasi_";
  std::string filename = "bin/generated/barabasiGenerated/barabasi_";
  filename.append(std::to_string(numberOfNodes));
  filename.append("_");
  filename.append(std::to_string(rand()%100));
  filename.append(".json");

  // std::string command = "python /home/mate/Documents/Programming/Research/MultilayerNetworkModel/MultilayerNetworkModel/sources/projects/BarabasiMultilayer/barabasiGenerator.py ";
  // std::string command = "python sources/projects/BarabasiMultilayer/barabasiGenerator.py ";
  std::string command = "python bin/utility/barabasiGenerator.py ";
  command.append(std::to_string(numberOfNodes));
  command.append(" ");
  command.append(filename);

  system(command.c_str());

  return filename;
}

void generateBarabasiNetwork(Network* network, int numberOfNodes, int &nodeIdCounter)
{
  std::string filename = pythonBarabasiGeneratorIndependent(numberOfNodes);
  loadNetworkFromJSON(network, filename, nodeIdCounter);
}
