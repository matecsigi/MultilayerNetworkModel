#ifndef MULTILAYERNETWORK_HH
#define MULTILAYERNETWORK_HH

#include "Node.hh"
#include "Layer.hh"
#include "IObserver.hh"
#include "GlobalVariables.hh"
#include "SimulationParameterContainer.hh"
#include <vector>
#include <iostream>
#include <map>

class MultilayerNetwork
{
public:
  MultilayerNetwork();
  ~MultilayerNetwork();

  /**
     Adds a layer to the multilayer network.
     @param layerId: the ID of the newly added layer
  */
  void addLayer(int layerId);
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

  //Save and load
  void save(std::string filename = "");
  void load(const char* filename);
  
  void saveState(std::string filename = "");
  void loadState(const char* filename);

  //Helper functions

  /**
     CalculationNodes with type ID need to contain the 
     pointer to the Node they represent. This function loads 
     all Node objects to their representing CalculationNodes.
   */
  void loadNodesToAllEquations(void);

  void updateNodesMap();

  /**
     When the end of buffers is reached i.e. they are filled 
     with the previous values of the nodes, the contents of the 
     current buffers is writen to a file, and the buffers are set 
     back to all 0 elements, except for the first two values that 
     contain the end values of the previous buffer.
   */
  void shiftBuffers(void);

  /**
     Find the maximum and minimum values among all the nodes in the multilayer network.
     There are used to check if the values go to +/- infinity.
   */
  double getMaxValue();
  double getMinValue();

  void collectNodes(std::map<int, Node*>& nodesMap, std::vector<int>& nodeIds) const;
  void collectNetworks(std::map<int, Network*>& networksMap, std::vector<int>& networkIds) const;

  void calculateClusterMessageSizes(SimulationParameterContainer *parameters);

  friend bool initialConditionsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);
  friend bool dynamicalEquationsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);
  friend std::ostream& operator<<(std::ostream& os, const MultilayerNetwork& multilayerNetwork);
  friend bool operator==(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);

  /**
     Contains all the nodes in id, pointer pairs.
     Used for executing the steps for the nodes.
   */
  std::map<int, Node*> mNodesMap;
  std::vector<int> mNodeIds;

private:
  std::vector<Layer*> mLayers;

};

void executeStepsInThread(std::vector<Node*> &nodes, SimulationParameterContainer *parameters);

#endif
