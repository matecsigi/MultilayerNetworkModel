
#ifndef MULTILAYERNETWORK_HH
#define MULTILAYERNETWORK_HH

#include "Node.hh"
#include "Layer.hh"
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
  void step(void);

  //Save and load
  void save(const char* filename);
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

  /**
     When the end of buffers is reached i.e. they are filled 
     with the previous values of the nodes, the contents of the 
     current buffers is writen to a file, and the buffers are set 
     back to all 0 elements, except for the first two values that 
     contain the end values of the previous buffer.
   */
  void shiftBuffers(void);

  void collectNodes(std::map<int, Node*>& nodesMap, std::vector<int>& nodeIds) const;
  void collectNetworks(std::map<int, Network*>& networksMap, std::vector<int>& networkIds) const;

  friend bool initialConditionsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);
  friend bool dynamicalEquationsEqual(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);
  friend std::ostream& operator<<(std::ostream& os, const MultilayerNetwork& multilayerNetwork);
  friend bool operator==(const MultilayerNetwork& multilayerNetwork1, const MultilayerNetwork& multilayerNetwork2);

private:
  std::vector<Layer*> mLayers;
};
#endif
