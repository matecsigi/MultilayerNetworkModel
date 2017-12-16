#include "VectorFieldSchemes.hh"
#include "IdValuePair.hh"
#include "UtilityFunctions.hh"

void gridAroundPointScheme(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates)
{
  std::vector<IdValuePair> directionAtState = network->getDirectionAtState(basePointCoordinates);
  vectorField->addPoint(basePointCoordinates, directionAtState);
  
  for(std::vector<IdValuePair>::iterator itCor=basePointCoordinates.begin(); itCor != basePointCoordinates.end(); ++itCor)
  {
    int id = itCor->mId;
    for(int i=1; i<3; ++i)
    {
      std::vector<IdValuePair> coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-i);
      std::vector<IdValuePair> direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
    }
    for(int i=1; i<3; ++i)
    {
      std::vector<IdValuePair> coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+i);
      std::vector<IdValuePair> direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
    }
  }
}
