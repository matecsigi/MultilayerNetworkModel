#ifndef NODE_HH
#define NODE_HH

#include "Network.hh"
#include "IUpwardInfluence.hh"
#include "IDownwardInfluence.hh"
#include "IVectorFieldReconfiguration.hh"
#include "GlobalVariables.hh"
#include "UtilityFunctions.hh"
#include <vector>

class IUpwardInfluence;
class IDownwardInfluence;
class IVectorFieldReconfiguration;

class Node
{
public:
  Node();
  Node(int);
  ~Node();

  int getId(void) const;

  /**
     Returns a vector containing pointers to all 
     the networks the node is part of.
     @return vector of networks a node is part of
   */
  std::vector<Network*> getNetworks(void) const;

  Network* getNetworkAssigned(void) const;
  void setNetworkAssigned(Network* network);
  
  /**
     The node is added to a network. Not called directly, but
     through Network::addNode().
     @param[int] network: the network the node is added to
   */
  void addToNetwork(Network* network);

  //State

  /**
     Sets the contents of the whole buffer and not just single values.
     @param[in] values: these values are set to the buffer
   */
  void setValues(double* values);
  void setCurrentState(state_type state);
  
  /**
     Stored the content of the whole buffer in the values
     variable.
     @param[out] values: all the states oof the buffer are stored here
   */
  void getValues(double* values);
  double getCurrentState();
  double getPreviousState();

  //Simulation
  /**
     Performs one step on the node involving the ODE, 
     UpwardInfluence, DownwardInfluence and VectorFieldReconfiguration.
     The current value of the node is used for the stepping.
   */
  void step();

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
  void stepOdeAtState(DynamicalEquation* dynamicalEquation, std::map<int, double> startingState, std::map<int, double>& finalState);

  /**
     Steps the dynamical equation. A node can have multiple dynamical equations,
     because it can be part of multiple network in the same layer. Only one of these 
     equations is stepped in this function.
     @param dynamicalEquation: the dynamical equation to be stepped
   */
  void stepODE(DynamicalEquation* dynamicalEquation);

  void setUpwardInfluence();
  void setDownwardInfluence();

  friend bool operator==(const Node& node1, const Node& node2);

private:
  int mNodeId;
  Network* mNetworkAssigned;
  std::vector<Network*> mNetworks;
  double* mValues;
  IUpwardInfluence* mUpwardInfluence;
  IDownwardInfluence* mDownwardInfluence;
  IVectorFieldReconfiguration* mVectorFieldReconfiguration;
};
#endif
