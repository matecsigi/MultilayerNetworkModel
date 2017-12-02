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
  Node* chooseNode(Network* network);
  Node* chooseNewNeighbor(Network* network, Node* node);
  Node* chooseFromNeighbors(Network* network, Node* node);
  ModificationType chooseType();

  //Modification functions
  void addEdge(Network* network, Node* node);
  void removeEdge(Network* network, Node* node);
  void addToOuterBlock(Network* network, Node* node);
  void removeFromOuterBlock(Network* network, Node* node);
  void changeConstant(DynamicalEquation* nodeEquation);
  void changePlusToMultiply(DynamicalEquation* nodeEquation);
  void changeMultiplyToPlus(DynamicalEquation* nodeEquation);

  //Helper functions
  int numberOfType(CalculationNode* calcNode, CalcNodeTypes type);
  void changeSpecifiedConstant(CalculationNode* calcNode, int elementIndex, int elementCounter);
  CalculationNode* changeSpecifiedPlusToMultiply(CalculationNode* baseCalcNode, int elementIndex);
  CalculationNode* changeSpecifiedMultiplyToPlus(CalculationNode* baseCalcNode, int elementIndex);
  void getSpecifiedElementFromType(CalculationNode* &storeCalcNode, CalculationNode* stepCalcNode, CalcNodeTypes type, int elementIndex, int elementCounter);
  CalculationNode* getParent(CalculationNode* calcNode, CalculationNode* childCalcNode);
  void getInsertLocations(CalculationNode* calcNode, std::vector<CalculationNode*> &locations);
  void getOuterInsertLocations(CalculationNode* calcNode, std::vector<CalculationNode*> &locations);
  void getNodeOccurrences(CalculationNode* calcNode, std::vector<CalculationNode*> &locations, int nodeId);

  CalculationNode* insertNodeAtLocation(CalculationNode* baseCalcNode, CalculationNode* calcNode, Node* node);
  CalculationNode* deleteNodeAtLocation(CalculationNode* baseCalcNode, CalculationNode* calcNode);
};

#endif
