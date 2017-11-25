#include "VectorFieldSchemes.hh"

void gridAroundPointScheme(VectorField* vectorField, Network* network, std::map<int, double> basePointCoordinates)
{
  std::map<int, double> directions = network->getDirectionAtState(basePointCoordinates);
}
