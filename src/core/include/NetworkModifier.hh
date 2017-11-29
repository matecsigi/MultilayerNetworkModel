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
  NetworkModifier(Network* network);
  ~NetworkModifier();

  void fitToVectorField(VectorField* targetVectorField);
  void modifyNetwork(Network* network);
  void copyNetwork(Network* oldNetwork, Network* newNetwork);
  Node* chooseNode(Network* network);
  ModificationType chooseType();

  //Modification functions
  void addEdge(DynamicalEquation* nodeEquation);
  void removeEdge(DynamicalEquation* nodeEquation);
  void addToOuterBlock(DynamicalEquation* nodeEquation);
  void removeFromOuterBlock(DynamicalEquation* nodeEquation);
  void changeConstant(DynamicalEquation* nodeEquation);
  void changePlusToMultiply(DynamicalEquation* nodeEquation);
  void changeMultiplyToPlus(DynamicalEquation* nodeEquation);

  //Helper functions
  int numberOfType(CalculationNode* calcNode, CalcNodeTypes type);
  void changeSpecifiedConstant(CalculationNode* calcNode, int elementIndex, int elementCounter);
  CalculationNode* changeSpecifiedPlusToMultiply(CalculationNode* baseCalcNode, int elementIndex);
  CalculationNode* changeSpecifiedMultiplyToPlus(CalculationNode* baseCalcNode, int elementIndex);
  void getSpecifiedElementFromType(CalculationNode* &storeCalcNode, CalculationNode* stepCalcNode, CalcNodeTypes type, int elementIndex, int elementCounter);
  CalculationNode* getParent(CalculationNode* calcNode, CalculationNode* parentCalcNode);
private:
  Network* mNetwork;
};

#endif