#ifndef NODE_HH
#define NODE_HH

#include "Network.hh"
#include "IUpwardInfluence.hh"
#include "IDownwardInfluence.hh"
#include "IVectorFieldReconfiguration.hh"
#include "GlobalVariables.hh"
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

  void step(); //set return type if necessary
  void stepODE(DynamicalEquation* dynamicalEquation);
  void stepOdeAtState(DynamicalEquation* dynamicalEquation, std::map<int, double> startingState, std::map<int, double>& finalState);

  void addToNetwork(Network* networkPtr);
  void assignToNetwork(Network* network);
  void setValues(double* values);
  void setUpwardInfluence();
  void setDownwardInfluence();
  void setInitialConditions(double* values);
  void setCurrentState(state_type state);

  int getId(void) const;
  Network* getNetworkAssigned(void) const;
  std::vector<Network*> getNetworks(void) const;
  void getValues(double* values);
  double getValue(void);
  void getInitialConditions(double* values);
  double getCurrentState();
  double getPreviousState();

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
