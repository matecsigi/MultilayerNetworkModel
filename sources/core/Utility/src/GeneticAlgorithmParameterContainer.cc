#include "GeneticAlgorithmParameterContainer.hh"

std::string GeneticAlgorithmParameterContainer::toString()
{
  std::string strParams;
  strParams.append("Genetic Parameters");
  strParams.append("\n\n");
  
  strParams.append("initialPopulationSize = ");
  strParams.append(std::to_string(initialPopulationSize));
  strParams.append("\n");

  strParams.append("numberOfGenerationa = ");
  strParams.append(std::to_string(numberOfGenerations));
  strParams.append("\n");

  strParams.append("mutationRatio = ");
  strParams.append(std::to_string(mutationRatio));
  strParams.append("\n");

  strParams.append("crossoverRatio = ");
  strParams.append(std::to_string(crossoverRatio));
  strParams.append("\n");

  strParams.append("deathRatio = ");
  strParams.append(std::to_string(deathRatio));
  strParams.append("\n");

  strParams.append("elitRatio = ");
  strParams.append(std::to_string(elitRatio));
  strParams.append("\n");

  strParams.append("maxConstantChange = ");
  strParams.append(std::to_string(maxConstantChange));
  strParams.append("\n");
  
  return strParams;
}
