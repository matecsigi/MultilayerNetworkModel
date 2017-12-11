#include "VectorFieldSchemes.hh"

void gridAroundPointScheme(VectorField* vectorField, Network* network, std::map<int, double> basePointCoordinates)
{
  std::map<int, double> tmpCoordinate;

  vectorField->addPoint(basePointCoordinates, network->getDirectionAtState(basePointCoordinates));
  
  for(std::map<int, double>::iterator itCor=basePointCoordinates.begin(); itCor != basePointCoordinates.end(); ++itCor)
  {
    int id = itCor->first;
    for(int i=1; i<3; ++i)
    {
      tmpCoordinate = basePointCoordinates;
      tmpCoordinate[id] = tmpCoordinate[id]-i;
      std::map<int, double> direction = network->getDirectionAtState(tmpCoordinate);
      vectorField->addPoint(tmpCoordinate, direction);
    }
    for(int i=1; i<3; ++i)
    {
      tmpCoordinate = basePointCoordinates;
      tmpCoordinate[id] = tmpCoordinate[id]+i;
      std::map<int, double> direction = network->getDirectionAtState(tmpCoordinate);
      vectorField->addPoint(tmpCoordinate, direction);
    }
  }
}
