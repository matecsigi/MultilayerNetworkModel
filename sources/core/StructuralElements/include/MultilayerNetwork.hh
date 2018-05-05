#ifndef MULTILAYERNETWORK_HH
#define MULTILAYERNETWORK_HH

#include "IObserver.hh"
#include "SimulationParameterContainer.hh"
#include <vector>
#include <map>

class Node;
class Layer;

class MultilayerNetwork
{
public:
  MultilayerNetwork();
  ~MultilayerNetwork();

  //------------------------------------
  //-----Basic------------------
  //------------------------------------  

  int getTime();
  void setTime(int time);

  Layer* addLayer();
  void addLayerById(int layerId);

  std::vector<Layer*> getLayers(void) const;

  /**
     Performs a single step on all of the nodes within
     the multilayer network.
   */
  void step(SimulationParameterContainer *parameters = NULL);
  /**
     Iterates the system for a given number of steps.
   */
  void iterate(int steps, SimulationParameterContainer *parameters = NULL, IObserver *observer = NULL);

  //------------------------------------
  //-----Save and load------------------
  //------------------------------------

  void save(std::string filename = "");
  void load(const char* filename);
  
  void saveState(std::string filename = "");
  void loadState(const char* filename);

  //------------------------------------
  //-----Utility------------------------
  //------------------------------------

  void print();

  /**
     Contains all the nodes in id, pointer pairs.
     Used for executing the steps for the nodes.
   */
  std::map<int, Node*> mNodesMap;
  std::vector<int> mNodeIds;

private:
  std::vector<Layer*> mLayers;
  int mTime;
};

void executeStepsInThread(std::vector<Node*> &nodes, SimulationParameterContainer *parameters);

#endif
