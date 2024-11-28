#include "wifi6_simulation.h"
#include <chrono>
#include <cmath>

// WiFi6 Access Point Implementation
WiFi6AccessPoint::WiFi6AccessPoint(const std::string& id) : WiFi5AccessPoint(id) {}

void WiFi6AccessPoint::initializeSubChannels() {
    m_subChannels = {
        {2.0, false}, {4.0, false}, {10.0, false}, {4.0, false} // Sub-channels totaling 20 MHz
    };
}

void WiFi6AccessPoint::allocateSubChannels(std::vector<User>& users) {
    size_t userIndex = 0;
    for (auto& subChannel : m_subChannels) {
        if (userIndex >= users.size()) break;
        subChannel.isOccupied = true;
        // Simulate allocation (no real packet logic for this demo)
        userIndex++;
    }
}

void WiFi6AccessPoint::performOFDMA(std::vector<User>& users) {
    initializeSubChannels();
    allocateSubChannels(users);

    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - startTime).count();
        if (elapsed >= m_ofdmaDuration) break;

        // Transmit packets for each allocated user
        for (auto& user : users) {
            if (user.hasPackets()) {
                try {
                    Packet<std::string> packet = user.getNextPacket();
                    user.recordTransmissionTime(std::chrono::steady_clock::now());
                } catch (const WiFiSimulationException& e) {
                    std::cerr << "Transmission error: " << e.what() << std::endl;
                }
            }
        }
    }
}

// WiFi6 Simulation Implementation
WiFi6Simulation::WiFi6Simulation(size_t userCount, const std::string& apId)
    : WiFi5Simulation(userCount, apId), 
      m_wifi6AccessPoint(apId) {}

void WiFi6Simulation::runSimulation() {
    const int MAX_ITERATIONS = 100;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        m_wifi6AccessPoint.performOFDMA(m_users);
    }
}

void WiFi6Simulation::printSimulationResults() {
    std::cout << "WiFi6 Simulation Results:\n";
    WiFi5Simulation::printSimulationResults();
}

// Factory method implementation
std::unique_ptr<WiFi6Simulation> createWiFi6Simulation(size_t userCount) {
    return std::make_unique<WiFi6Simulation>(userCount);
}
