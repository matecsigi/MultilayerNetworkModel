#include "interpreter.h"
#include <sstream>
#include <boost/concept_check.hpp>

using namespace EzAquarii;

Interpreter::Interpreter() :
    m_scanner(*this),
    m_parser(m_scanner, *this),
    m_location(0)
{

}

int Interpreter::parse() {
    m_location = 0;
    return m_parser.parse();
}

CalculationNode* Interpreter::getBaseCalculationNode()
{
  return mCalcBase;
}

void Interpreter::switchInputStream(std::istringstream *is)
{
    m_scanner.switch_streams(is, NULL);
}

void Interpreter::increaseLocation(unsigned int loc) {
    m_location += loc;
}

unsigned int Interpreter::location() const {
    return m_location;
}
