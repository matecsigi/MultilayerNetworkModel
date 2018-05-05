#ifndef IDOWNWARDINFLUENCE_HH
#define IDOWNWARDINFLUENCE_HH

class MultilayerNetwork;
class Node;

class IDownwardInfluence
{
public:
  IDownwardInfluence(Node* node){mNode = node;};
  virtual ~IDownwardInfluence(void){};

  virtual void calculateDownwardInfluence(double downwardMultiplier) = 0;
protected:
  Node* mNode;
};

#endif
