#ifndef WIFI6_SIMULATION_H
#define WIFI6_SIMULATION_H

#include "wifi5_simulation.h"
#include <queue>
#include <vector>

class WiFi6AccessPoint : public WiFi5AccessPoint {
private:
    struct SubChannel {
        double bandwidth; // Bandwidth of the sub-channel
        bool isOccupied;  // Is this sub-channel currently occupied
    };

    std::vector<SubChannel> m_subChannels; // List of sub-channels
    const double m_ofdmaDuration = 5.0;    // Duration for OFDMA scheduling (ms)

public:
    WiFi6AccessPoint(const std::string& id);

    // Initialize sub-channels based on 20 MHz bandwidth
    void initializeSubChannels();

    // Allocate sub-channels to users
    void allocateSubChannels(std::vector<User>& users);

    // Perform OFDMA transmission
    void performOFDMA(std::vector<User>& users);
};

class WiFi6Simulation : public WiFi5Simulation {
private:
    WiFi6AccessPoint m_wifi6AccessPoint;

public:
    WiFi6Simulation(size_t userCount, const std::string& apId = "AP1");

    void runSimulation() override;
    void printSimulationResults() override;
};

// Factory method to create WiFi6 simulation
std::unique_ptr<WiFi6Simulation> createWiFi6Simulation(size_t userCount);

#endif // WIFI6_SIMULATION_H
