#ifndef LAYER_HH
#define LAYER_HH

#include "Node.hh"
#include <vector>

class Layer
{
public:
  Layer();
  Layer(int);
  ~Layer();

  void addNode(int);

private:
  int mLayerId;
  std::vector<Node*> mNodesInLayer;
};
#endif
