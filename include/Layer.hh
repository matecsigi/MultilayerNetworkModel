#ifndef LAYER_HH
#define LAYER_HH

#include "Node.hh"
#include <vector>

class Layer
{
public:
  Layer();
  ~Layer();

private:
  int mLayerId;
  std::vector<Node*> mNodesInLayer;
};
#endif
