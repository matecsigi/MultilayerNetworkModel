#include "VectorFieldSchemes.hh"
#include "IdValuePair.hh"
#include "UtilityFunctions.hh"

void gridAroundPointScheme1(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates)
{
  std::vector<IdValuePair> directionAtState = network->getDirectionAtState(basePointCoordinates);
  vectorField->addPoint(basePointCoordinates, directionAtState);

  int counter = 0;
  
  for(std::vector<IdValuePair>::iterator itCor=basePointCoordinates.begin(); itCor != basePointCoordinates.end(); ++itCor)
  {
    int id = itCor->mId;
    for(int i=1; i<3; ++i)
    {
      std::vector<IdValuePair> coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-i);
      std::vector<IdValuePair> direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
      ++counter;
    }
    for(int i=1; i<3; ++i)
    {
      std::vector<IdValuePair> coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+i);
      std::vector<IdValuePair> direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
      ++counter;
    }
  }
  std::cout<<"scheme counter = "<<counter<<std::endl;
}

void gridAroundPointScheme2(VectorField* vectorField, Network* network, std::vector<IdValuePair> &basePointCoordinates)
{
  std::vector<IdValuePair> directionAtState = network->getDirectionAtState(basePointCoordinates);
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
      direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
    
      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-1);
      direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
    }

    // +/- 10
    if(counter%4 == 0)
    {
      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+10);
      direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);

      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-10);
      direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);      
    }

    // +/- 50
    if(counter%8 == 0)
    {
      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)+50);
      direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);

      coordinate = basePointCoordinates;
      setValueForId(coordinate, id, getValueForId(coordinate, id)-50);
      direction = network->getDirectionAtState(coordinate);
      vectorField->addPoint(coordinate, direction);
    }

    ++counter;
  }
}
