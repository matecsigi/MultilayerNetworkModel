#ifndef NODE_HH
#define NODE_HH

#include <vector>

class Node
{
public:
  Node();
  Node(int);
  ~Node();

  void step(); //set return type if necessary

  int getId(void) const;

private:
  int mNodeId;
  std::vector<Node*> mNodesUp;
  std::vector<Node*> mNodesDown;
  std::vector<Node*> mNodesOnLayer;
};
#endif
