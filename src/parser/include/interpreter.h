#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include "scanner.h"
#include "parser.hpp"

class CalculationNode;

namespace EzAquarii {

/* class CalculationNode; */

class Interpreter
{
public:
    Interpreter();
    
    int parse();
    CalculationNode* getBaseCalculationNode();
    void switchInputStream(std::istringstream *is);
    void printCalculationNodes(); 

    friend class Parser;
    friend class Scanner;
    
private:
    void increaseLocation(unsigned int loc);
    unsigned int location() const;
    
private:
    Scanner m_scanner;
    Parser m_parser;
    CalculationNode *mCalcBase;
    unsigned int m_location;
};

}

#endif
