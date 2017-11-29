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
  void modifyNetwork(Network* oldNetwork, Network* newNetwork);
  void copyNetwork(Network* oldNetwork, Network* newNetwork);
  Node* chooseNode(Network* network);
  ModificationType chooseType();

  void addEdge(Node* nodeToChange);
  void removeEdge(Node* nodeToChange);
  void addToOuterBlock(Node* nodeToChange);
  void removeFromOuterBlock(Node* nodeToChange);
  void changeConstant(Node* nodeToChange);
  void changePlusToMultiply(Node* nodeToChange);
  void changeMultiplyToPlus(Node* nodeToChange);
private:
  Network* mNetwork;
};

#endif
