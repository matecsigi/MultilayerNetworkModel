#include "Layer.hh"
#include <iostream>

using namespace std;

Layer::Layer(void)
{
  cout<<"Creating layer"<<endl;
}

Layer::Layer(int id)
{
  mLayerId = id;
  cout<<"Creating layer with id="<<mLayerId<<endl;
}

Layer::~Layer(void)
{
  for(std::vector<Node*>::iterator it = mNodesInLayer.begin(); it != mNodesInLayer.end(); ++it)
  {
    delete (*it);
  }
}

void Layer::addNode(int nodeId)
{
  mNodesInLayer.push_back(new Node(nodeId));
}
