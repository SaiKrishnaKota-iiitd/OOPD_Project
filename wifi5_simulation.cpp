#include "wifi5_simulation.h"
#include <algorithm>
#include <chrono>

// WiFi5 Access Point Implementation
WiFi5AccessPoint::WiFi5AccessPoint(const std::string& id)
    : AccessPoint(id), 
      m_currentUserIndex(0),
      m_generator(std::random_device{}()) {}

void WiFi5AccessPoint::broadcastInitialPacket() {
    // Simulate broadcast packet for multi-user MIMO setup
    Packet<std::string> broadcastPacket("MIMO_SETUP", 0.5); // Small packet
    // std::cout << "Broadcast Initial MIMO Setup Packet\n";
}

void WiFi5AccessPoint::collectChannelStateInfo(std::vector<User>& users) {
    m_csiPackets.clear();
    
    // Collect 200-byte CSI packets from each user
    for (auto& user : users) {
        Packet<std::string> csiPacket("CSI_" + user.getId(), 0.2); // 200 bytes
        m_csiPackets.push_back(csiPacket);
        // std::cout << "Collected CSI from " << user.getId() << "\n";
    }
}

void WiFi5AccessPoint::performMultiUserMIMOTransmission(std::vector<User>& users) {
    // Round-robin transmission for 15ms
    auto startTime = std::chrono::steady_clock::now();
    
    while (true) {
        // Check if 15ms have passed
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - startTime
        ).count();
        
        if (elapsed >= 15) break;

        // Round-robin transmission
        if (m_currentUserIndex >= users.size()) {
            m_currentUserIndex = 0;
        }

        User& currentUser = users[m_currentUserIndex];
        
        // Attempt transmission for current user
        try {
            if (currentUser.hasPackets()) {
                Packet<std::string> packet = currentUser.getNextPacket();
                
                // Simulate parallel transmission
                // std::cout << "Parallel transmission for " 
                //           << currentUser.getId() << "\n";
                
                // Record transmission time
                currentUser.recordTransmissionTime(
                    std::chrono::steady_clock::now()
                );
            }
        }
        catch (const WiFiSimulationException& e) {
            std::cerr << "Transmission error: " << e.what() << std::endl;
        }

        // Move to next user
        m_currentUserIndex++;
    }
}

bool WiFi5AccessPoint::tryMultiUserTransmission(User* user) {
    // WiFi5 specific transmission logic
    // This method can be customized further if needed
    return true;
}

// WiFi5 Simulation Implementation
WiFi5Simulation::WiFi5Simulation(size_t userCount, const std::string& apId)
    : WiFi4Simulation(userCount, apId), 
      m_wifi5AccessPoint(apId) {}

void WiFi5Simulation::runSimulation() {
    const int MAX_ITERATIONS = 100;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        // WiFi5 specific simulation flow
        
        // 1. Broadcast initial packet
        m_wifi5AccessPoint.broadcastInitialPacket();
        
        // 2. Collect Channel State Information
        m_wifi5AccessPoint.collectChannelStateInfo(m_users);
        
        // 3. Perform Multi-User MIMO transmission
        m_wifi5AccessPoint.performMultiUserMIMOTransmission(m_users);
    }
}

void WiFi5Simulation::printSimulationResults() {
    std::cout << "WiFi5 Simulation Results:\n";
    
    // Calculate and print throughput and latency
    std::cout << "Max Theoretical Throughput: " 
              << m_wifi5AccessPoint.calculateMaxThroughput() << " Mbps\n";

    // Calculate latency for each user
    float avg_latency = 0.0;
    float max_latency = 0.0;
    for (const auto& user : m_users) {
        const auto& transmissionTimes = user.getTransmissionTimes();
        
        if (transmissionTimes.size() > 1) {
            auto totalLatency = std::chrono::duration_cast<std::chrono::microseconds>(
                transmissionTimes.back() - transmissionTimes.front()
            ).count();
            avg_latency += totalLatency / transmissionTimes.size();
            max_latency = max_latency > (totalLatency / transmissionTimes.size()) ? max_latency : (totalLatency / transmissionTimes.size());
            // std::cout << user.getId() << " Average Latency: " 
            //           << (totalLatency / transmissionTimes.size()) << " microseconds\n";
        }
    }
    std::cout<< "Average Latency of "<< m_users.size() << " Users: " << avg_latency / m_users.size() << " microseconds\n";
    std::cout<< "Max Latency of "<< m_users.size() << " Users: " << max_latency << " microseconds\n";
}

// Factory method implementation
std::unique_ptr<WiFi5Simulation> createWiFi5Simulation(size_t userCount) {
    return std::make_unique<WiFi5Simulation>(userCount);
}