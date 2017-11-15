#include <iostream>
#include <sstream>
#include "scanner.h"
#include "parser.hpp"
#include "interpreter.h"

using namespace EzAquarii;
using namespace std;

int main(int argc, char **argv) {
    Interpreter i;
    std::string myString = "1+(1+ID1)";
    std::istringstream myStream(myString);
    i.switchInputStream(&myStream);
    int res = i.parse();
    cout << "Parse complete. Result = " << res << endl;
    i.printCalculationNodes();
    return res;
}
