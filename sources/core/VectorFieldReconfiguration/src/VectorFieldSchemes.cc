#include "VectorFieldSchemes.hh"

void gridAroundPointScheme(VectorField* vectorField, Network* network, std::map<int, double> basePointCoordinates)
{
  vectorField->addPoint(basePointCoordinates, network->getDirectionAtState(basePointCoordinates));
  
  for(std::map<int, double>::iterator itCor=basePointCoordinates.begin(); itCor != basePointCoordinates.end(); ++itCor)
  {
    int id = itCor->first;
    for(int i=1; i<3; ++i)
    {
      std::map<int, double> coordinate = basePointCoordinates;
      coordinate[id] = coordinate[id]-i;
      std::map<int, double> direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
    }
    for(int i=1; i<3; ++i)
    {
      std::map<int, double> coordinate = basePointCoordinates;
      coordinate[id] = coordinate[id]+i;
      std::map<int, double> direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
    }
  }
}
