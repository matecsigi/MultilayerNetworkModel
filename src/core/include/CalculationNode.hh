#ifndef CALCULATIONNODE_HH
#define CALCULATIONNODE_HH

#include <cmath>
#include <iostream>

using namespace std;

class Node;

enum CalcNodeTypes {
  CONSTANT,
  ID,
  NEGATE,
  ADD,
  SUBSTRACT,
  MULTIPLY,
  DIVIDE,
  POWER
};

class CalculationNode
{
public:
  CalculationNode(){};
  virtual ~CalculationNode(){};

  virtual double evaluate() const = 0;
  virtual bool testNodeIds() const = 0;
  virtual std::string toString() const = 0;

  CalcNodeTypes getType(){return type;};
  virtual int getId(){return -1;};

  virtual void setNode(Node* node){};

  CalculationNode* left;
  CalculationNode* right;
protected:
  CalcNodeTypes type;
};

class CNConstant : public CalculationNode
{
public:
  explicit CNConstant(double _value) : CalculationNode(), value(_value)
  {
    left = NULL;
    right = NULL;
    type = CONSTANT;
  }

  virtual double evaluate() const
  {
    return value;
  }

  virtual bool testNodeIds() const
  {
    return true;
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append(std::to_string(value));
    return strCalcNode;
  }
private:
  double value;
};


class CNId : public CalculationNode
{
public:
    explicit CNId(int _id)
	: CalculationNode(), id(_id)
    {
      left = NULL;
      right = NULL;
      type = ID;
      mNode = NULL;
    }

  int getId()
  {
    return id;
  }
  
  void setNode(Node* node)
  {
    mNode = node;
  }

  virtual double evaluate() const;

  virtual bool testNodeIds() const;

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("ID");
    strCalcNode.append(std::to_string(id));
    return strCalcNode;
  }
private:
  int id;
  Node* mNode;
};


class CNNegate : public CalculationNode
{
public:
  explicit CNNegate(CalculationNode* _right)
  {
    right = _right;
    left = NULL;
    type = NEGATE;
  }

  virtual ~CNNegate()
  {
    delete right;
  }

  virtual double evaluate() const
  {
    return - right->evaluate();
  }

  virtual bool testNodeIds() const
  {
    return right->testNodeIds();
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("(");
    strCalcNode.append("-");
    strCalcNode.append(right->toString());
    strCalcNode.append(")");
    return strCalcNode;
  }
};


class CNAdd : public CalculationNode
{    
public:
  explicit CNAdd(CalculationNode* _left, CalculationNode* _right)
  {
    left = _left;
    right = _right;
    type = ADD;
  }

  virtual ~CNAdd()
  {
    delete left;
    delete right;
  }

  virtual double evaluate() const
  {
    return left->evaluate() + right->evaluate();
  }

  virtual bool testNodeIds() const
  {
    return ((left->testNodeIds()) && (right->testNodeIds()));
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("(");
    strCalcNode.append(left->toString());
    strCalcNode.append("+");
    strCalcNode.append(right->toString());
    strCalcNode.append(")");
    return strCalcNode;
  }
};


class CNSubtract : public CalculationNode
{    
public:
  explicit CNSubtract(CalculationNode* _left, CalculationNode* _right)
  {
    left = _left;
    right = _right;
    type = SUBSTRACT;
  }

  virtual ~CNSubtract()
  {
    delete left;
    delete right;
  }

  virtual double evaluate() const
  {
    return left->evaluate() - right->evaluate();
  }

  virtual bool testNodeIds() const
  {
    return ((left->testNodeIds()) && (right->testNodeIds()));
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("(");
    strCalcNode.append(left->toString());
    strCalcNode.append("-");
    strCalcNode.append(right->toString());
    strCalcNode.append(")");
    return strCalcNode;
  }
};


class CNMultiply : public CalculationNode
{    
public:
  explicit CNMultiply(CalculationNode* _left, CalculationNode* _right)
  {
    left = _left;
    right = _right;
    type = MULTIPLY;
  }

  virtual ~CNMultiply()
  {
    delete left;
    delete right;
  }

  virtual double evaluate() const
  {
    return left->evaluate() * right->evaluate();
  }

  virtual bool testNodeIds() const
  {
    return ((left->testNodeIds()) && (right->testNodeIds()));
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("(");
    strCalcNode.append(left->toString());
    strCalcNode.append("*");
    strCalcNode.append(right->toString());
    strCalcNode.append(")");
    return strCalcNode;
  }
};


class CNDivide : public CalculationNode
{    
public:
  explicit CNDivide(CalculationNode* _left, CalculationNode* _right)
  {
    left = _left;
    right = _right;
    type = DIVIDE;
  }

  virtual ~CNDivide()
  {
    delete left;
    delete right;
  }

  virtual double evaluate() const
  {
    return left->evaluate() / right->evaluate();
  }

  virtual bool testNodeIds() const
  {
    return ((left->testNodeIds()) && (right->testNodeIds()));
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("(");
    strCalcNode.append(left->toString());
    strCalcNode.append("/");
    strCalcNode.append(right->toString());
    strCalcNode.append(")");
    return strCalcNode;
  }
};


class CNPower : public CalculationNode
{    
public:
  explicit CNPower(CalculationNode* _left, CalculationNode* _right)
  {
    left = _left;
    right = _right;
    type = POWER;
  }
  
  virtual ~CNPower()
  {
    delete left;
    delete right;
  }

  virtual double evaluate() const
  {
    return std::pow(left->evaluate(), right->evaluate());
  }

  virtual bool testNodeIds() const
  {
    return ((left->testNodeIds()) && (right->testNodeIds()));
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("(");
    strCalcNode.append(left->toString());
    strCalcNode.append("^");
    strCalcNode.append(right->toString());
    strCalcNode.append(")");
    return strCalcNode;
  }
};

#endif
