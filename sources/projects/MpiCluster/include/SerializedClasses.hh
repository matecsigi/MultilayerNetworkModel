#ifndef SERIALIZEDCLASSES_HH
#define SERIALIZEDCLASSES_HH

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include "Network.hh"
#include "VectorField.hh"

class SerializedNode
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mId;
    ar & mDynamicalEquation;
  }

public:
  SerializedNode(){};
  SerializedNode(int id, std::string dynamicalEquation):mId(id), mDynamicalEquation(dynamicalEquation){};
  ~SerializedNode(){};

  int mId;
  std::string mDynamicalEquation;
};

class SerializedEdge
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mSourceId;
    ar & mTargetId;
  }

public:
  SerializedEdge(){};
  SerializedEdge(int sourceId, int targetId):mSourceId(sourceId), mTargetId(targetId){};
  ~SerializedEdge(){};

  int mSourceId;
  int mTargetId;
};

class SerializedNetwork
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mNodes;
    ar & mEdges;
  }

  std::vector<SerializedNode> mNodes;
  std::vector<SerializedEdge> mEdges;
public:
  SerializedNetwork(){};
  ~SerializedNetwork(){};

  friend void serializeNetwork(Network *network, SerializedNetwork *serializedNetwork);
  friend void deserializeNetwork(SerializedNetwork* serializedNetwork, Network* network);
};

class SerializedIdValuePair
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mId;
    ar & mValue;
  }

public:
  SerializedIdValuePair(){};
  SerializedIdValuePair(int id, double value): mId(id), mValue(value){};
  ~SerializedIdValuePair(){};

  int mId;
  double mValue;
};

class SerializedVectorFieldPoint
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mCoordinate;
    ar & mDirection;
  }

public:
  SerializedVectorFieldPoint(){};
  SerializedVectorFieldPoint(std::vector<SerializedIdValuePair> &coordinate, std::vector<SerializedIdValuePair> &direction):mCoordinate(coordinate), mDirection(direction){};
  ~SerializedVectorFieldPoint(){};

  std::vector<SerializedIdValuePair> mCoordinate;
  std::vector<SerializedIdValuePair> mDirection;
};

class SerializedVectorField
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mVectorFieldPoints;
  }

  std::vector<SerializedVectorFieldPoint> mVectorFieldPoints;

public:
  SerializedVectorField(){};
  ~SerializedVectorField(){};

  friend void serializeVectorField(VectorField *vectorField, SerializedVectorField *serializedVectorField);
  friend void deserializeVectorField(SerializedVectorField *serializedVectorField, VectorField *vectorField);
};

class GeneticAlgorithmMessage
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mNodeId;
    ar & mNumberOfRequests;
    ar & mNetwork;
    ar & mVectorField;
  }

public:
  GeneticAlgorithmMessage(){};
  GeneticAlgorithmMessage(int id):mNodeId(id){};
  ~GeneticAlgorithmMessage(){};
  int getNodeId(){return mNodeId;};

  int mNodeId;
  int mNumberOfRequests;
  SerializedNetwork mNetwork;
  SerializedVectorField mVectorField;
};

class GeneticAlgorithmReply
{
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
    ar & mNodeId;
    ar & mNetwork;
  }

public:
  GeneticAlgorithmReply(){};
  GeneticAlgorithmReply(int id):mNodeId(id){};
  ~GeneticAlgorithmReply(){};
  int getNodeId(){return mNodeId;};

  int mNodeId;
  SerializedNetwork mNetwork;
};

void serializeNetwork(Network *network, SerializedNetwork *serializedNetwork);
void deserializeNetwork(SerializedNetwork* serializedNetwork, Network* network);

void serializeVectorField(VectorField *vectorField, SerializedVectorField *serializedVectorField);
void deserializeVectorField(SerializedVectorField *serializedVectorField, VectorField *vectorField);

#endif
