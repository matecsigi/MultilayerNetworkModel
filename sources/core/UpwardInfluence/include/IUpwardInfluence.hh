#ifndef IUPWARDINFLUENCE_HH
#define IUPWARDINFLUENCE_HH

class MultilayerNetwork;
class Node;

class IUpwardInfluence
{
public:
  IUpwardInfluence(Node* node){mNode = node;};
  virtual ~IUpwardInfluence(void){};

  virtual void calculateUpwardInfluence(double upwardMultiplier) = 0;
protected:
  Node* mNode;
};

#endif
