#ifndef NETWORKMODIFIER_HH
#define NETWORKMODIFIER_HH

#include "Node.hh"
#include "Network.hh"
#include "VectorField.hh"
#include "GeneticAlgorithmParameterContainer.hh"

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
  NetworkModifier(GeneticAlgorithmParameterContainer *geneticParameters = 0);
  ~NetworkModifier();

  void modifyNetwork(Network* network, int numberOfChanges=1);

  //Chooser functions
  Node* chooseNode(Network* network);
  ModificationType chooseType();

  //Modification functions
  /**
     Adds a new edge to the node by connecting it to a node it the network it wasn't connected before.
     The variable representing the new neighbor is inserted at a random location to the node's equation. The variable is inserted with a constant variable.
   */
  void addEdge(Network* network, Node* node);

  /**
     Removes an edge from the node. The occurences of the variable representing the old neighbor are 
     all deleted from the equation along with thier constant multipliers.
   */
  void removeEdge(Network* network, Node* node);

  /**
     Chooses one of the neighbors of the node and adds its variable again to the equation.
     The variable together with a costant multiplier is inserted to the outer block of the equation.
     The outer block is where all entities are connected with + e.g. (ID1*ID2)+ID3, the entities in the outer block are (ID1+ID2) and ID3.
     Note that the structure of the netwotk doesn't change i.e. no edge is added or removed, but it makes it possible to have multiple ocurences of a variable within the equation.
   */
  void addToOuterBlock(Network* network, Node* node);

  /**
     The opposite of addToOuterBlock(). A node variable together with its constant multiplier is removed from the outer block, but only if there are other occurences of that variable in the equation, so the edge between the two nodes doesn't get removed.
     Note that the structure of the network doesn't change i.e. no edge is addded or removed.
   */
  void removeFromOuterBlock(Network* network, Node* node);

  /**
     Randomly chooses a constant value in the equation and changes its value.
     The constant value is either a multiplier of one of the node variables or just stands in itself (there should be only one constant standing in itself in the equation).
   */
  void changeConstant(Network* network, Node* node);

  /**
     Randomly chooses an addition operator in the equation and changes it to multiplication.
     The addition of the constant that stands in itself cannot be changed to multiplication, because we would like to preserve a constant in the equation that stands in itself.
   */
  void changePlusToMultiply(Network* network, Node* node);

  /**
     The opposite of changePlusToMultiply(). Randomly chooses a multiplication operator in the equation and changes it to addition.
     The multiplication connecting a node variable to its constant multiplier cannot be changes to addition, because we want that each node variable would have a constant multiplier.
   */
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
  CalculationNode* specific_removeEdge(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode, bool &deletionCancelled);
  void specific_changeConstant(CalculationNode* changingCalcNode);
  CalculationNode* specific_changePlusToMultiply(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode);
  CalculationNode* specific_changeMultiplyToPlus(CalculationNode* baseCalcNode, CalculationNode* changingCalcNode);

  //Helper functions
  int numberOfType(CalculationNode* calcNode, CalcNodeTypes type);
  CalculationNode* getParent(CalculationNode* calcNode, CalculationNode* childCalcNode);
  void getNodeOccurrences(CalculationNode* calcNode, std::vector<CalculationNode*> &locations, int nodeId);
  double getNodeMultiplier(CalculationNode* calcNode, int nodeId);
  void setNodeMultiplier(CalculationNode* calcNode, int nodeId, double value);

private:
  double mMaxConstantChange;
  std::vector<double> mModificationTypeProbabilities;
  int mDefaultCall;
};

#endif
