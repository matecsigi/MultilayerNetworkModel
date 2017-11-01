#include "Node.hh"
#include <iostream>

using namespace std;

Node::Node(void)
{
  cout<<"New node created"<<endl;
}

Node::Node(int id)
{
  mId = id;
  cout<<"New node with id="<<mId<<endl;
}

Node::~Node(void)
{
}

void Node::step(void)
{
  cout<<"Stepping node"<<endl;
}
