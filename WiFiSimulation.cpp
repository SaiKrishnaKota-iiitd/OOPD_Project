#include "WiFiSimulation.h"

// User Class Implementation
Packet<std::string> User::getNextPacket() {
    if (m_packetQueue.empty()) {
        throw WiFiSimulationException("No packets available");
    }
    
    Packet<std::string> packet = m_packetQueue.front();
    m_packetQueue.pop();
    return packet;
}

// Access Point Implementation
double AccessPoint::calculateMaxThroughput() const {
    // Calculate theoretical max throughput based on WiFi 4 parameters
    // Bandwidth * Modulation Order * Coding Rate
    return m_channel.getBandwidth() * 
           std::log2(m_modulationOrder) * 
           m_codingRate;
}

bool AccessPoint::tryTransmit(User* user) {
    if (!user->hasPackets()) return false;

    // Check if channel is free
    if (!m_channel.isChannelFree()) {
        // Backoff mechanism
        int backoffTime = m_channel.getBackoffTime();
        return false;
    }

    // Occupy the channel
    m_channel.occupy();

    // Transmit packet
    try {
        Packet<std::string> packet = user->getNextPacket();
        
        // Record transmission time
        auto transmissionTime = std::chrono::steady_clock::now();
        user->recordTransmissionTime(transmissionTime);

        // Release channel
        m_channel.release();
        return true;
    }
    catch (const WiFiSimulationException& e) {
        std::cerr << "Transmission error: " << e.what() << std::endl;
        m_channel.release();
        return false;
    }
}

// WiFi4 Simulation Implementation
WiFi4Simulation::WiFi4Simulation(size_t userCount, const std::string& apId)
    : m_accessPoint(apId) {
    // Create users
    for (size_t i = 0; i < userCount; ++i) {
        m_users.emplace_back("User" + std::to_string(i));
        
        // Add some packets to each user
        for (int j = 0; j < 10; ++j) {
            m_users[i].addPacket(Packet<std::string>("Data" + std::to_string(j)));
        }
        
        // Connect users to access point
        m_accessPoint.addUser(&m_users[i]);
    }
}

void WiFi4Simulation::runSimulation() {
    const int MAX_ITERATIONS = 1000;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter) {
        // Attempt transmission for each user
        for (auto& user : m_users) {
            m_accessPoint.tryTransmit(&user);
        }
    }
}

void WiFi4Simulation::printSimulationResults() {
    // Calculate and print throughput and latency
    std::cout << "Simulation Results:\n";
    std::cout << "Max Theoretical Throughput: " 
              << m_accessPoint.calculateMaxThroughput() << " Mbps\n";

    float avg_latency = 0.0;
    float max_latency = 0.0;
    // Calculate latency for each user
    for (const auto& user : m_users) {
        const auto& transmissionTimes = user.getTransmissionTimes();
        
        if (transmissionTimes.size() > 1) {
            auto totalLatency = std::chrono::duration_cast<std::chrono::microseconds>(
                transmissionTimes.back() - transmissionTimes.front()
            ).count();
            max_latency = max_latency > (totalLatency / transmissionTimes.size()) ? max_latency : (totalLatency / transmissionTimes.size());
            avg_latency += totalLatency / transmissionTimes.size();
            // std::cout << user.getId() << " Average Latency: " 
            //           << (totalLatency / transmissionTimes.size()) << " microseconds\n";
        }
    }
    std::cout<< "Average Latency of "<< m_users.size() << " Users: " << avg_latency / m_users.size() << " microseconds\n";
    std::cout<< "Max Latency of "<< m_users.size() << " Users: " << max_latency << " microseconds\n";
}
