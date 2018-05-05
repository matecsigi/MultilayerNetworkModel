#include "VectorFieldReconfigurationImpl.hh"
#include "VectorField.hh"
#include "VectorFieldSchemes.hh"
#include "VectorFieldTransformImplementations.hh"
#include "MultilayerNetwork.hh"
#include "Node.hh"
#include "VectorField.hh"
#include "GeneticAlgorithmController.hh"
#include "MpiUtility.hh"
#include "SerializedClasses.hh"
#include <boost/mpi.hpp>

void VectorFieldReconfigurationImpl::calculateVectorFieldReconfiguration(SimulationParameterContainer *parameters)
{
  Network* networkAssigned = mNode->getNetworkAssigned();
  if(networkAssigned == NULL)
  {
    return;
  }

  GeneticAlgorithmParameterContainer *geneticParameters = parameters->geneticParameters;

  VectorField* currentVectorField = new VectorField();
  std::vector<IdValuePair> currentState = networkAssigned->getState();
  gridAroundPointScheme2(currentVectorField, networkAssigned, currentState);

  VectorField* targetVectorField = new VectorField();
  calculateTargetVectorField(targetVectorField, currentVectorField, mNode, parameters);

  // std::cout<<"--------Current---------"<<std::endl;;
  // std::cout<<*currentVectorField;
  // std::cout<<"--------Target----------"<<std::endl;
  // std::cout<<*targetVectorField;

  if(geneticParameters->cluster == true)
  {
    // std::cout<<"impl "<<mNode->getId()<<std::endl;
    int argc;
    char **argv = NULL;
    boost::mpi::environment env{argc, argv};
    boost::mpi::communicator world;

    SerializedNetwork serializedNetwork;
    serializeNetwork(networkAssigned, &serializedNetwork);
    SerializedVectorField serializedVectorField;
    serializeVectorField(targetVectorField, &serializedVectorField);

    GeneticAlgorithmMessage message(mNode->getId());
    int rankToProcess = (mNode->getId()%(world.size()-1))+1;
    message.mNumberOfRequests = geneticParameters->clusterMessageSizes[rankToProcess];
    // std::cout<<"nrOfReq="<<message.mNumberOfRequests<<std::endl;
    message.mNetwork = serializedNetwork;
    message.mVectorField = serializedVectorField;
    world.send(rankToProcess, 0, message);
    
    // std::cout<<"impl sent "<<mNode->getId()<<std::endl;
    // mpiSend(1, 0, mNode->getId());
  }
  else
  {
    GeneticAlgorithmController geneticController(geneticParameters);
    geneticController.fitToVectorField(networkAssigned, targetVectorField);
  }

  delete currentVectorField;
  delete targetVectorField;
}
