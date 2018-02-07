#ifndef GENERALNETWORKGENERATOR_HH
#define GENERALNETWORKGENERATOR_HH

#include <functional>
#include "Network.hh"

void generateNetwork(Network* network, int control, std::function<void(Network*)> structureGenerator = nullptr, std::function<void(Network*)> dynamicsGenerator = nullptr, std::function<void(Network*)> initialConditionGenerator = nullptr);

#endif
