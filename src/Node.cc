#include "Node.hh"
#include <iostream>

using namespace std;

Node::Node(void)
{
  cout<<"New node created"<<endl;
}

Node::~Node(void)
{
  cout<<"Node destroyed"<<endl;
}

void Node::step(void)
{
  cout<<"Stepping node"<<endl;
}
