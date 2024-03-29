#include "HebbianFitnessFunction.hh"
#include "NetworkPopulationElement.hh"
#include "Node.hh"
#include "Network.hh"
#include "Layer.hh"
#include "MultilayerNetwork.hh"
#include "NetworkInitialConditionGenerators.hh"
#include "BarabasiModel.hh"
#include "NetworkDynamicsGenerators.hh"
#include "MultilayerNetwork.hh"
#include "NetworkUtilityFunctions.hh"
#include "HebbianObserver.hh"
#include "SimulationParameterContainer.hh"
#include "MultilayerNetworkClassUtility.hh"
#include <iostream>
#include <random>
#include "spdlog/spdlog.h"

double hebbianFitnessFunction(Network* network, HebbianParameterContainer *hebbianParameters)
{
  double sumFitness = 0;

  for(int i=0; i < hebbianParameters->numberOfIterations; ++i)
  {
    MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;
    generateMultilayerNetworkForHebbianFitness(multilayerNetwork, network, hebbianParameters);

    SimulationParameterContainer *parameters = new SimulationParameterContainer;
    parameters->cluster = hebbianParameters->cluster;
    parameters->geneticParameters->cluster = hebbianParameters->cluster;
    parameters->geneticParameters->modificationTypeProbabilities = hebbianModTypeProbabilities;
    parameters->geneticParameters->initialPopulationSize = 30;

    IObserver *observer = new HebbianObserver(multilayerNetwork);

    multilayerNetwork->iterate(hebbianParameters->transientTime, parameters);
    multilayerNetwork->iterate(hebbianParameters->runTime, parameters, observer);

    double sumDistance = observer->getResult();
    double distance = sumDistance/(double)(hebbianParameters->runTime);
    double fitness = (double)100/distance;
    if((getMaxValue(multilayerNetwork) > 1.0e+30) || (getMinValue(multilayerNetwork) < -1.0e+30))
    {
      fitness = 1.0e-50;
    }
    
    sumFitness += fitness;

    // auto logger = spdlog::basic_logger_mt("hebbian_logger", "bin/generated/log.txt");
    // logger->info("fitness={}", fitness);
    // spdlog::drop_all();

    std::cout<<"fitness="<<fitness<<std::endl;

    delete observer;
    delete multilayerNetwork;
    delete parameters;
  }
  // std::cout<<"============="<<std::endl;

  return sumFitness/(double)hebbianParameters->numberOfIterations;
}

void generateMultilayerNetworkForHebbianFitness(MultilayerNetwork* multilayerNetwork, Network* network, HebbianParameterContainer *hebbianParameters)
{
  Layer* layer1 = multilayerNetwork->addLayer();
  Layer* layer2 = multilayerNetwork->addLayer();

  Network* insertedNetworkUp = layer1->insertNetwork(network);
  randomNetworkInitialConditions(insertedNetworkUp, hebbianParameters->higherNetworkInitConditionMin, hebbianParameters->higherNetworkInitConditionMax);  

  std::vector<Node*> nodesUp = insertedNetworkUp->getNodes();
  for(std::vector<Node*>::iterator itNode=nodesUp.begin(); itNode != nodesUp.end(); ++itNode)
  {
    Network *networkDown = new Network;
    generateNetwork(networkDown, hebbianParameters->lowerNetworkSize);
    Network* insertedNetworkDown = layer2->insertNetwork(networkDown);
    linearNetworkDynamicsGenerator(insertedNetworkDown);
    randomNetworkInitialConditions(insertedNetworkDown, hebbianParameters->lowerNetworkInitConditionMin, hebbianParameters->lowerNetworkInitConditionMax);
    (*itNode)->setNetworkAssigned(insertedNetworkDown);
  }
}
