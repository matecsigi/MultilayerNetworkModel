#ifndef GENERALNETWORKGENERATOR_HH
#define GENERALNETWORKGENERATOR_HH

#include <functional>

class Network;

void generateNetworkGeneral(Network* network, int control, std::function<void(Network*)> structureGenerator = nullptr, std::function<void(Network*)> dynamicsGenerator = nullptr, std::function<void(Network*)> initialConditionGenerator = nullptr);

#endif
