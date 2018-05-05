#include "VectorFieldSchemes.hh"
#include "VectorField.hh"
#include "Network.hh"
#include "NetworkUtilityFunctions.hh"
#include "IdValuePair.hh"
#include "UtilityFunctions.hh"
#include "NetworkUtilityFunctions.hh"

void gridAroundPointScheme1(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates)
{
  std::vector<IdValuePair> directionAtState = getIsolatedDirectionAtState(network, basePointCoordinates);
  vectorField->addPoint(basePointCoordinates, directionAtState);

  int counter = 0;
  
  for(std::vector<IdValuePair>::iterator itCor=basePointCoordinates.begin(); itCor != basePointCoordinates.end(); ++itCor)
  {
    int id = itCor->mId;
    for(int i=1; i<3; ++i)
    {
      std::vector<IdValuePair> coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-i);
      std::vector<IdValuePair> direction = getIsolatedDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);
      ++counter;
    }
    for(int i=1; i<3; ++i)
    {
      std::vector<IdValuePair> coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+i);
      std::vector<IdValuePair> direction = getIsolatedDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);
      ++counter;
    }
  }
  std::cout<<"scheme counter = "<<counter<<std::endl;
}

void gridAroundPointScheme2(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates, std::function<std::vector<IdValuePair> (Network*, std::vector<IdValuePair>&)> getDirectionAtState)
{
  std::vector<IdValuePair> directionAtState = getDirectionAtState(network, basePointCoordinates);
  vectorField->addPoint(basePointCoordinates, directionAtState);

  int counter = 0;

  for(std::vector<IdValuePair>::iterator itCor=basePointCoordinates.begin(); itCor != basePointCoordinates.end(); ++itCor)
  {
    int id = itCor->mId;
    std::vector<IdValuePair> coordinate;
    std::vector<IdValuePair> direction;

    // +/- 1
    if(counter%2 ==0)
    {
      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+1);
      direction = getDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);
    
      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-1);
      direction = getDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);
    }

    // +/- 10
    if(counter%4 == 0)
    {
      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+10);
      direction = getDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);

      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-10);
      direction = getDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);      
    }

    // +/- 50
    if(counter%8 == 0)
    {
      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+50);
      direction = getDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);

      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-50);
      direction = getDirectionAtState(network, coordinate);
      vectorField->addPoint(coordinate, direction);
    }

    ++counter;
  }
}
