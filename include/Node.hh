#ifndef NODE_HH
#define NODE_HH

#include <vector>

class Node
{
public:
  Node();
  ~Node();
  void step(); //set return type if necessary

private:
  int mId;
  std::vector<Node*> mNodesUp;
  std::vector<Node*> mNodesDown;
  std::vector<Node*> mNodesOnLayer;
};
#endif
