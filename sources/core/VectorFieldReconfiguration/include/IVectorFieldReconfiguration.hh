#ifndef IVECTORFIELDRECONFIGURATION_HH
#define IVECTORFIELDRECONFIGURATION_HH

#include "MultilayerNetwork.hh"
#include "GlobalVariables.hh"

class IVectorFieldReconfiguration
{
public:
  IVectorFieldReconfiguration(Node* node, std::vector<double> &modificationTypeProbabilities = vectorReconfModTypeProbabilities){mNode = node; mModificationTypeProbabilities = modificationTypeProbabilities;};
  virtual ~IVectorFieldReconfiguration(void){};

  virtual void calculateVectorFieldReconfiguration(std::vector<double> &modificationTypeProbabilities = vectorReconfModTypeProbabilities) = 0;
protected:
  Node* mNode;
  std::vector<double> mModificationTypeProbabilities;
};

#endif
