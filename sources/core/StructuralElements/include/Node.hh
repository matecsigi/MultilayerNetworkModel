#ifndef NODE_HH
#define NODE_HH

#include "GlobalVariables.hh"
#include "SimulationParameterContainer.hh"
#include <vector>

class IUpwardInfluence;
class IDownwardInfluence;
class IVectorFieldReconfiguration;

class MultilayerNetwork;
class Network;
class DynamicalEquation;

class IdValuePair;

class Node
{
public:
  Node();
  Node(int);
  ~Node();

  //------------------------------------
  //-----Basic--------------------------
  //------------------------------------

  int getId(void) const;
  void setId(int id);

  int getTime();
  
  std::vector<Network*> getNetworks(void) const;

  Network* getNetworkAssigned(void) const;
  void setNetworkAssigned(Network* network);
  
  /**
     The node is added to a network. Not called directly, but
     through Network::addNode().
     @param[int] network: the network the node is added to
   */
  void addToNetwork(Network* network);

  void getValues(double* values) const;
  void setValues(double* values);

  double getCurrentState();
  void setCurrentState(double state);
  
  double getPreviousState();

  double getChangeByUpwardInfluence(int index);
  void setChangeByUpwardInfluence(int index, double value);

  double getChangeByDownwardInfluence(int index);
  void setChangeByDownwardInfluence(int index, double value);

  void setMultilayerNetwork(MultilayerNetwork *multilayerNetwork);

  //------------------------------------
  //-----Simulation---------------------
  //------------------------------------
  /**
     Performs one step on the node involving the ODE, 
     UpwardInfluence, DownwardInfluence and VectorFieldReconfiguration.
     The current value of the node is used for the stepping.
   */
  void step(SimulationParameterContainer *parameters);

  /**
     Steps the dynamical equation. A node can have multiple dynamical equations,
     because it can be part of multiple network in the same layer. Only one of these 
     equations is stepped in this function.
     @param dynamicalEquation: the dynamical equation to be stepped
   */
  void stepODE(DynamicalEquation* dynamicalEquation, SimulationParameterContainer *parameters);

  /**
     Performs one step on the node involving only the ODE.
     The state given is used as the starting state of stepping.
     The dynamical equation is given as a parameter, because 
     the ODE is only stepped for one equation, but a node could 
     be part of multiply networks with different equations.
     @param[in] dynamicalEquation: the dynamical equation to be stepped
     @param[in] startingState: describes the state of the node to be taken as the basis for stepping
     @param[out] finalState: the result of the stepping (final value) is stored here
   */
  void stepOdeAtState(DynamicalEquation* dynamicalEquation, std::vector<IdValuePair> &startingState, std::vector<IdValuePair> &finalState);

  //------------------------------------
  //-----Utility------------------------
  //------------------------------------

  void print();
  
private:
  int mNodeId;
  Network* mNetworkAssigned;
  std::vector<Network*> mNetworks;
  double* mValues;
  IUpwardInfluence* mUpwardInfluence;
  IDownwardInfluence* mDownwardInfluence;
  IVectorFieldReconfiguration* mVectorFieldReconfiguration;

  /**
     Stores the changes due to the last exectution of the 
     UpwardInfluence function on the node. This is needed because 
     these changes are only propagated to one direction to avoid 
     oscillation between nodes on neighbor layers that would result in 
     their values going to infinity.
   */
  std::vector<double> mChangeByUpwardInfluence;

  /**
     Stores the changes due to the exectution of the 
     UpwardInfluence function on the node.
   */
  std::vector<double> mChangeByDownwardInfluence;
  MultilayerNetwork *mMultilayerNetwork;
};
#endif
