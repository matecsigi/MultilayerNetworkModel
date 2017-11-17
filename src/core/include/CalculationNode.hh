#ifndef CALCULATIONNODE_HH
#define CALCULATIONNODE_HH

#include <cmath>
#include <iostream>

using namespace std;

// namespace EzAquarii {

class CalculationNode
{
public:
  CalculationNode(){};
  virtual ~CalculationNode(){};

  virtual double evaluate() const = 0;
  virtual void print() const = 0;
  virtual std::string toString() const = 0;
};

class CNConstant : public CalculationNode
{
public:
    explicit CNConstant(double _value)
	: CalculationNode(), value(_value)
    {
    }

    virtual double evaluate() const
    {
	return value;
    }
  
  virtual void print() const
  {
    cout<<"const "<<evaluate()<<endl;
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
    }

    virtual double evaluate() const
    {
	return 1;
    }

  virtual void print() const
  {
    cout<<"id "<<evaluate()<<endl;
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("ID");
    strCalcNode.append(std::to_string(id));
    return strCalcNode;
  }
private:
  int id;
};


class CNNegate : public CalculationNode
{
public:
    explicit CNNegate(CalculationNode* _node)
	: CalculationNode(), node(_node)
    {
    }

    virtual ~CNNegate()
    {
	delete node;
    }

    virtual double evaluate() const
    {
	return - node->evaluate();
    }

  virtual void print() const
  {
    cout<<"negate "<<evaluate()<<endl;
    node->print();
  }

  virtual std::string toString() const
  {
    std::string strCalcNode;
    strCalcNode.append("(");
    strCalcNode.append("-");
    strCalcNode.append(node->toString());
    strCalcNode.append(")");
    return strCalcNode;
  }
private:
    CalculationNode* node;
};


class CNAdd : public CalculationNode
{    
public:
    explicit CNAdd(CalculationNode* _left, CalculationNode* _right)
	: CalculationNode(), left(_left), right(_right)
    {
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

  virtual void print() const
  {
    cout<<"add "<<evaluate()<<endl;
    left->print();
    right->print();
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
private:
    CalculationNode* left;
    CalculationNode* right;
};


class CNSubtract : public CalculationNode
{    
public:
    explicit CNSubtract(CalculationNode* _left, CalculationNode* _right)
	: CalculationNode(), left(_left), right(_right)
    {
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

  virtual void print() const
  {
    cout<<"substract "<<evaluate()<<endl;
    left->print();
    right->print();
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
private:
    CalculationNode* left;
    CalculationNode* right;
};


class CNMultiply : public CalculationNode
{    
public:
    explicit CNMultiply(CalculationNode* _left, CalculationNode* _right)
	: CalculationNode(), left(_left), right(_right)
    {
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

  virtual void print() const
  {
    cout<<"multiply "<<evaluate()<<endl;
    left->print();
    right->print();
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
private:
    CalculationNode* left;
    CalculationNode* right;
};


class CNDivide : public CalculationNode
{    
public:
    explicit CNDivide(CalculationNode* _left, CalculationNode* _right)
	: CalculationNode(), left(_left), right(_right)
    {
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

  virtual void print() const
  {
    cout<<"divide "<<evaluate()<<endl;
    left->print();
    right->print();
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
private:
    CalculationNode* left;
    CalculationNode* right;
};


class CNPower : public CalculationNode
{    
public:
    explicit CNPower(CalculationNode* _left, CalculationNode* _right)
	: CalculationNode(), left(_left), right(_right)
    {
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

  virtual void print() const
  {
    cout<<"power"<<evaluate()<<endl;
    left->print();
    right->print();
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
private:
    CalculationNode* left;
    CalculationNode* right;

};

//}

#endif
