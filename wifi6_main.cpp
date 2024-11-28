#include "wifi6_simulation.h"

int main() {
    // Case (i): 1 user and 1 AP
    {
        auto simulation = createWiFi6Simulation(1);
        simulation->runSimulation();
        std::cout << "Case (i): 1 User\n";
        simulation->printSimulationResults();
    }

    // Case (ii): 10 users and 1 AP
    {
        auto simulation = createWiFi6Simulation(10);
        simulation->runSimulation();
        std::cout << "Case (ii): 10 Users\n";
        simulation->printSimulationResults();
    }

    // Case (iii): 100 users and 1 AP
    {
        auto simulation = createWiFi6Simulation(100);
        simulation->runSimulation();
        std::cout << "Case (iii): 100 Users\n";
        simulation->printSimulationResults();
    }

    return 0;
}
