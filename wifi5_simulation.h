#ifndef WIFI5_SIMULATION_H
#define WIFI5_SIMULATION_H

#include "WiFiSimulation.h"

class WiFi5AccessPoint : public AccessPoint {
private:
    // CSI (Channel State Information) specific attributes
    std::vector<Packet<std::string>> m_csiPackets;
    const double m_multiUserMIMODuration = 15.0; // ms
    
    // Round-robin scheduling attributes
    size_t m_currentUserIndex;
    std::mt19937 m_generator;

public:
    WiFi5AccessPoint(const std::string& id);

    // Broadcast initial packet for multi-user MIMO setup
    void broadcastInitialPacket();

    // Collect Channel State Information (CSI) from users
    void collectChannelStateInfo(std::vector<User>& users);

    // Perform multi-user MIMO transmission
    void performMultiUserMIMOTransmission(std::vector<User>& users);

    // Additional WiFi5 specific transmission method
    bool tryMultiUserTransmission(User* user);
};

// Extended Simulation Class for WiFi5
class WiFi5Simulation : public WiFi4Simulation {
private:
    WiFi5AccessPoint m_wifi5AccessPoint;

public:
    WiFi5Simulation(size_t userCount, const std::string& apId = "AP1");

    // Override base class methods
    void runSimulation() override;
    void printSimulationResults() override;
};

// Factory method to create WiFi5 simulation
std::unique_ptr<WiFi5Simulation> createWiFi5Simulation(size_t userCount);

#endif // WIFI5_SIMULATION_H