#include <boost/test/unit_test.hpp>
#include "MultilayerNetwork.hh"
#include "Layer.hh"
#include "Network.hh"
#include "Node.hh"
#include "DynamicalEquation.hh"
#include "NetworkModifier.hh"
#include "IStructureGenerator.hh"
#include "StructureGeneratorImpl.hh"
#include "IDynamicalEquationGenerator.hh"
#include "DynamicalEquationGeneratorSimpleImpl.hh"

BOOST_AUTO_TEST_SUITE(TestSuiteNetworkModifier)

BOOST_AUTO_TEST_CASE(NetworkModifierAddEdge)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
  structureGenerator->generateStructure(6, 3);

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<Network*> networks = layers[0]->getNetworks();
  Network* network = networks[0];
  NetworkModifier* networkModifier = new NetworkModifier();
  Node* node = networkModifier->chooseNode(network);

  DynamicalEquation* testEquation = network->getNodeDynamicalEquation(node->getId());
  int originalNumberOfAdds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ADD);
  int originalNumberOfIds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ID);
  networkModifier->addEdge(network, node);
  testEquation = network->getNodeDynamicalEquation(node->getId());
  int newNumberOfAdds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ADD);
  int newNumberOfIds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ID);

  bool operationCorrect = false;
  if((originalNumberOfAdds == (newNumberOfAdds-1)) && (originalNumberOfIds == (newNumberOfIds-1)))
  {
    operationCorrect = true;
  }

  BOOST_CHECK_MESSAGE(operationCorrect == true, "Adding edge in vector field reconfiguration doesn't work");

  delete multilayerNetwork;
}

BOOST_AUTO_TEST_CASE(networkModifier_removeEdge)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
structureGenerator->generateStructure(6, 3);

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<Network*> networks = layers[0]->getNetworks();
  Network* network = networks[0];
  NetworkModifier* networkModifier = new NetworkModifier();
  Node* node = networkModifier->chooseNode(network);

  DynamicalEquation* testEquation = network->getNodeDynamicalEquation(node->getId());
  int originalNumberOfIds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ID);
  networkModifier->removeEdge(network, node);
  testEquation = network->getNodeDynamicalEquation(node->getId());
  int newNumberOfIds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ID);

  bool operationCorrect = false;
  if(originalNumberOfIds == (newNumberOfIds+1))
  {
    operationCorrect = true;
  }

  BOOST_CHECK_MESSAGE(operationCorrect == true, "Removing edge in vector field reconfiguration doesn't work");

  delete multilayerNetwork;
}

BOOST_AUTO_TEST_CASE(networkModifier_addToOuterBlock)
{
  MultilayerNetwork* multilayerNetwork = new MultilayerNetwork;

  IStructureGenerator* structureGenerator = new StructureGeneratorImpl(multilayerNetwork);
structureGenerator->generateStructure(6, 3);

  IDynamicalEquationGenerator* dynamicalEquationGenerator = new DynamicalEquationGeneratorSimpleImpl(multilayerNetwork);
  dynamicalEquationGenerator->generateDynamicalEquations();

  std::vector<Layer*> layers = multilayerNetwork->getLayers();
  std::vector<Network*> networks = layers[0]->getNetworks();
  Network* network = networks[0];
  NetworkModifier* networkModifier = new NetworkModifier();
  Node* node = networkModifier->chooseNode(network);

  DynamicalEquation* testEquation = network->getNodeDynamicalEquation(node->getId());
  int originalNumberOfAdds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ADD);
  int originalNumberOfIds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ID);
  networkModifier->addToOuterBlock(network, node);
  testEquation = network->getNodeDynamicalEquation(node->getId());
  int newNumberOfAdds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ADD);
  int newNumberOfIds = networkModifier->numberOfType(testEquation->getBaseCalculationNode(), ID);

  bool operationCorrect = false;
  if((originalNumberOfAdds == (newNumberOfAdds-1)) && (originalNumberOfIds == (newNumberOfIds-1)))
  {
    operationCorrect = true;
  }

  BOOST_CHECK_MESSAGE(operationCorrect == true, "Adding to outer block in vector field reconfiguration doesn't work");

  delete multilayerNetwork;
}

BOOST_AUTO_TEST_CASE(networkModifier_changePlusToMultiply)
{
  Network* network = new Network;
  network->addNodeById(1);
  std::string strEquation = "(1+1)+(1+1)";
  network->setDynamicalEquationString(1, strEquation);
  std::vector<Node*> nodes = network->getNodes();
  Node* node = nodes[0];
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(1);

  NetworkModifier* networkModifier = new NetworkModifier();

  int originalNumberOfAdds = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), ADD);
  int originalNumberOfMultiplies = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), MULTIPLY);
  networkModifier->changePlusToMultiply(network, node);
  int newNumberOfAdds = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), ADD);
  int newNumberOfMultiplies = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), MULTIPLY);

  bool operationCorrect = false;
  if((originalNumberOfAdds == (newNumberOfAdds+1)) && (originalNumberOfMultiplies == (newNumberOfMultiplies-1)))
  {
    operationCorrect = true;
  }

  BOOST_CHECK_MESSAGE(operationCorrect == true, "Changing ADDs to MULTIPLYs in equations doesn't work");

  delete network;
}

BOOST_AUTO_TEST_CASE(networkModifier_changeMultiplyToPlus)
{
  Network* network = new Network;
  network->addNodeById(1);
  std::string strEquation = "(1*ID1)*(1*ID2)";
  network->setDynamicalEquationString(1, strEquation);
  std::vector<Node*> nodes = network->getNodes();
  Node* node = nodes[0];
  DynamicalEquation* nodeEquation = network->getNodeDynamicalEquation(1);

  NetworkModifier* networkModifier = new NetworkModifier();

  int originalNumberOfAdds = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), ADD);
  int originalNumberOfMultiplies = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), MULTIPLY);
  networkModifier->changeMultiplyToPlus(network, node);
  int newNumberOfAdds = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), ADD);
  int newNumberOfMultiplies = networkModifier->numberOfType(nodeEquation->getBaseCalculationNode(), MULTIPLY);

  bool operationCorrect = false;
  if((originalNumberOfAdds == (newNumberOfAdds-1)) && (originalNumberOfMultiplies == (newNumberOfMultiplies+1)))
  {
    operationCorrect = true;
  }

  BOOST_CHECK_MESSAGE(operationCorrect == true, "Changing MULTIPLYs to ADDs in equations doesn't work");
  
  delete network;
}

BOOST_AUTO_TEST_SUITE_END()
