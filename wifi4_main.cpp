#include "WiFiSimulation.h"

int main() {
    try {
        // Test Case 1: 1 User, 1 AP
        std::cout << "Simulation with 1 User and 1 AP:\n";
        WiFi4Simulation sim1(1);
        sim1.runSimulation();
        sim1.printSimulationResults();

        std::cout << "\n---\n";

        // Test Case 2: 10 Users, 1 AP
        std::cout << "Simulation with 10 Users and 1 AP:\n";
        WiFi4Simulation sim2(10);
        sim2.runSimulation();
        sim2.printSimulationResults();

        std::cout << "\n---\n";

        // Test Case 3: 100 Users, 1 AP
        std::cout << "Simulation with 100 Users and 1 AP:\n";
        WiFi4Simulation sim3(100);
        sim3.runSimulation();
        sim3.printSimulationResults();

        return 0;
    }
    catch (const WiFiSimulationException& e) {
        std::cerr << "Simulation Error: " << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Unexpected Error: " << e.what() << std::endl;
        return 1;
    }
}
