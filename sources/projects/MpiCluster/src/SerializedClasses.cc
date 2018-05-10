#include "SerializedClasses.hh"
#include "Node.hh"
#include "VectorFieldPoint.hh"
#include "IdValuePair.hh"

void serializeNode(Node *node, SerializedNode *serializedNode)
{
  double *values = new double[bufferSize];
  node->getValues(values);
  for(int i=0; i<bufferSize; ++i)
  {
    serializedNode->mValues.push_back(values[i]);
  }

  delete values;
}

void deserializeNode(SerializedNode *serializedNode, Node *node)
{
  double *values = new double[bufferSize];
  int counter = 0;
  for(std::vector<double>::iterator itValue=serializedNode->mValues.begin(); itValue != serializedNode->mValues.end(); ++itValue)
  {
    values[counter] = *itValue;
    ++counter;
  }
  node->setValues(values);
  delete values;
}

void serializeNetwork(Network *network, SerializedNetwork *serializedNetwork)
{
  std::vector<Node*> nodes = network->getNodes();
  for(std::vector<Node*>::iterator itNode=nodes.begin(); itNode != nodes.end(); ++itNode)
  {
    Node* currentNode = (*itNode);
    int id = currentNode->getId();
    std::string dynamicalEquation = network->getNodeDynamicalEquationString(id);
    SerializedNode serializedNode(id, dynamicalEquation);
    serializeNode(currentNode, &serializedNode);
    serializedNetwork->mNodes.push_back(serializedNode);
    
    std::vector<Node*> neighbors = network->getNodeNeighbors(id);
    for(std::vector<Node*>::iterator itNei=neighbors.begin(); itNei != neighbors.end(); ++itNei)
    {
      int neighborId = (*itNei)->getId();
      serializedNetwork->mEdges.push_back(SerializedEdge(id, neighborId));
    }
  }

  serializedNetwork->mTime = network->getTime();
}

void deserializeNetwork(SerializedNetwork* serializedNetwork, Network* network)
{
  std::vector<SerializedNode> serializedNodes = serializedNetwork->mNodes;
  for(std::vector<SerializedNode>::iterator itNode=serializedNodes.begin(); itNode != serializedNodes.end(); ++itNode)
  {
    SerializedNode currentNode = (*itNode);
    int id = currentNode.mId;
    std::string dynamicalEquation = currentNode.mDynamicalEquation;
    network->addNodeById(id);
    network->setDynamicalEquationString(id, dynamicalEquation);
    Node *node = network->getNodeById(id);
    deserializeNode(&currentNode, node);
  }

  std::vector<SerializedEdge> serializedEdges = serializedNetwork->mEdges;
  for(std::vector<SerializedEdge>::iterator itEdge=serializedEdges.begin(); itEdge != serializedEdges.end(); ++itEdge)
  {
    SerializedEdge currentEdge = (*itEdge);
    int source = currentEdge.mSourceId;
    int target = currentEdge.mTargetId;
    network->addEdge(source, target);
  }

  network->setTime(serializedNetwork->mTime);
}

void serializeVectorField(VectorField *vectorField, SerializedVectorField *serializedVectorField)
{
  std::vector<VectorFieldPoint*> vectorFieldPoints = vectorField->getVectorFieldPoints();
  for(std::vector<VectorFieldPoint*>::iterator itPoint=vectorFieldPoints.begin(); itPoint != vectorFieldPoints.end(); ++itPoint)
  {
    VectorFieldPoint *currentPoint = (*itPoint);

    std::vector<SerializedIdValuePair> serializedCoordinate;
    std::vector<IdValuePair> coordinate = currentPoint->getCoordinate();
    for(std::vector<IdValuePair>::iterator itCor=coordinate.begin(); itCor!= coordinate.end(); ++itCor)
    {
      int id = (*itCor).mId;
      double value = (*itCor).mValue;
      serializedCoordinate.push_back(SerializedIdValuePair(id, value));
    }

    std::vector<SerializedIdValuePair> serializedDirection;
    std::vector<IdValuePair> direction = currentPoint->getDirection();
    for(std::vector<IdValuePair>::iterator itDir=direction.begin(); itDir != direction.end(); ++itDir)
    {
      int id = (*itDir).mId;
      double value = (*itDir).mValue;
      serializedDirection.push_back(SerializedIdValuePair(id, value));      
    }

    serializedVectorField->mVectorFieldPoints.push_back(SerializedVectorFieldPoint(serializedCoordinate, serializedDirection));
  }
}

void deserializeVectorField(SerializedVectorField *serializedVectorField, VectorField *vectorField)
{
  std::vector<SerializedVectorFieldPoint> serializedPoints = serializedVectorField->mVectorFieldPoints;
  for(std::vector<SerializedVectorFieldPoint>::iterator itPoint=serializedPoints.begin(); itPoint != serializedPoints.end(); ++itPoint)
  {
    SerializedVectorFieldPoint currentPoint = *itPoint;

    std::vector<IdValuePair> coordinate;
    std::vector<SerializedIdValuePair> serializedCoordinate = currentPoint.mCoordinate;
    for(std::vector<SerializedIdValuePair>::iterator itCor=serializedCoordinate.begin(); itCor != serializedCoordinate.end(); ++itCor)
    {
      int id = (*itCor).mId;
      double value = (*itCor).mValue;
      coordinate.push_back(IdValuePair(id, value));
    }

    std::vector<IdValuePair> direction;
    std::vector<SerializedIdValuePair> serializedDirection = currentPoint.mDirection;
    for(std::vector<SerializedIdValuePair>::iterator itDir=serializedDirection.begin(); itDir != serializedDirection.end(); ++itDir)
    {
      int id = (*itDir).mId;
      double value = (*itDir).mValue;
      direction.push_back(IdValuePair(id, value));
    }

    vectorField->addPoint(coordinate, direction);
  }
}
