#ifndef NETWORKMODIFIER_HH
#define NETWORKMODIFIER_HH

#include "Node.hh"
#include "Network.hh"
#include "VectorField.hh"

enum ModificationType
{
  ADD_EDGE,
  REMOVE_EDGE,
  ADD_TO_OUTER_BLOCK,
  REMOVE_FROM_OUTER_BLOCK,
  CHANGE_CONSTANT,
  CHANGE_PLUS_TO_MULTIPLY,
  CHANGE_MULTIPLY_TO_PLUS
};

class NetworkModifier
{
public:
  NetworkModifier();
  ~NetworkModifier();

  void modifyNetwork(Network* network);
  void copyNetwork(Network* oldNetwork, Network* newNetwork);

  //Chooser functions
  Node* chooseNode(Network* network);
  ModificationType chooseType();

  //Modification functions
  void addEdge(Network* network, Node* node);
  void removeEdge(Network* network, Node* node);
  void addToOuterBlock(Network* network, Node* node);
  void removeFromOuterBlock(Network* network, Node* node);
  void changeConstant(Network* network, Node* node);
  void changePlusToMultiply(Network* network, Node* node);
  void changeMultiplyToPlus(Network* network, Node* node);

  //getNode function
  Node* getNode_addEdge(Network* network, Node* node);
  Node* getNode_removeEdge(Network* network, Node* node);

  //getLocations functions
  void getLocations_addEdge(CalculationNode* calcNode, std::vector<CalculationNode*> &locations);
  void getLocations_addToOuterBlock(CalculationNode* calcNode, std::vector<CalculationNode*> &locations);
  void getLocations_removeFromOuterBlock(CalculationNode* baseCalcNode, CalculationNode* calcNode, std::vector<CalculationNode*> &locations);
  void getLocations_changeConstant(CalculationNode* calcNode, std::vector<CalculationNode*> &locations);
  void getLocations_changePlusToMultiply(CalculationNode* calcNode, std::vector<CalculationNode*> &locations);
  void getLocations_changeMultiplyToPlus(CalculationNode* calcNode, std::vector<CalculationNode*> &locations);

  //Specific change functions (given the exact location of change)
  CalculationNode* specific_addEdge(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode, Node* newNeighbor);
  CalculationNode* specific_removeEdge(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode);
  void specific_changeConstant(CalculationNode* changingCalcNode);
  CalculationNode* specific_changePlusToMultiply(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode);
  CalculationNode* specific_changeMultiplyToPlus(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode);

  //Helper functions
  int numberOfType(CalculationNode* calcNode, CalcNodeTypes type);
  CalculationNode* getParent(CalculationNode* calcNode, CalculationNode* childCalcNode);
  void getNodeOccurrences(CalculationNode* calcNode, std::vector<CalculationNode*> &locations, int nodeId);
};

#endif
