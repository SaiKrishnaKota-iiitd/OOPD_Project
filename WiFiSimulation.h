#ifndef WIFI_SIMULATION_H
#define __WIFI_SIMULATION_H

#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <cmath>

// Forward declarations
template <typename T>
class Packet;

template <typename T>
class FrequencyChannel;

class User;
class AccessPoint;

// Exception class for WiFi simulation errors
class WiFiSimulationException : public std::runtime_error {
public:
    explicit WiFiSimulationException(const std::string& message)
        : std::runtime_error(message) {}
};

// Abstract base class for network entities
class NetworkEntity {
protected:
    std::string m_id;

public:
    NetworkEntity(const std::string& id) : m_id(id) {}
    virtual ~NetworkEntity() = default;
    std::string getId() const { return m_id; }
};

// Packet Template Class
template <typename T>
class Packet {
private:
    T m_data;
    size_t m_size;  // in KB
    std::chrono::steady_clock::time_point m_creationTime;

public:
    Packet(const T& data, size_t size = 1) 
        : m_data(data), m_size(size), 
          m_creationTime(std::chrono::steady_clock::now()) {}

    size_t getSize() const { return m_size; }
    auto getCreationTime() const { return m_creationTime; }
};

// Frequency Channel Template Class
template <typename T>
class FrequencyChannel {
private:
    double m_bandwidth;  // in MHz
    bool m_isOccupied;
    std::mt19937 m_generator;
    std::uniform_int_distribution<> m_backoffDistribution;

public:
    FrequencyChannel(double bandwidth = 20.0) 
        : m_bandwidth(bandwidth), 
          m_isOccupied(false), 
          m_generator(std::random_device{}()),
          m_backoffDistribution(0, 15) {}

    bool isChannelFree() const { return !m_isOccupied; }
    void occupy() { m_isOccupied = true; }
    void release() { m_isOccupied = false; }

    int getBackoffTime() {
        return m_backoffDistribution(m_generator);
    }

    double getBandwidth() const { return m_bandwidth; }
};

// User Class
class User : public NetworkEntity {
private:
    std::queue<Packet<std::string>> m_packetQueue;
    std::vector<std::chrono::steady_clock::time_point> m_transmissionTimes;

public:
    User(const std::string& id) : NetworkEntity(id) {}

    void addPacket(const Packet<std::string>& packet) {
        m_packetQueue.push(packet);
    }

    bool hasPackets() const { return !m_packetQueue.empty(); }
    Packet<std::string> getNextPacket();

    void recordTransmissionTime(const std::chrono::steady_clock::time_point& time) {
        m_transmissionTimes.push_back(time);
    }

    const std::vector<std::chrono::steady_clock::time_point>& getTransmissionTimes() const {
        return m_transmissionTimes;
    }
};

// Access Point Class
class AccessPoint : public NetworkEntity {
private:
    FrequencyChannel<std::string> m_channel;
    std::vector<User*> m_connectedUsers;
    std::mt19937 m_generator;
    std::uniform_real_distribution<> m_probabilityDistribution;

    // Modulation and coding parameters
    const int m_modulationOrder = 256;  // 256-QAM
    const double m_codingRate = 5.0 / 6.0;

public:
    AccessPoint(const std::string& id) 
        : NetworkEntity(id), 
          m_channel(20.0),
          m_generator(std::random_device{}()),
          m_probabilityDistribution(0.0, 1.0) {}

    void addUser(User* user) {
        m_connectedUsers.push_back(user);
    }

    FrequencyChannel<std::string>& getChannel() { return m_channel; }

    double calculateMaxThroughput() const;
    
    bool tryTransmit(User* user);
};

class WiFiSimulation{
    public:
        virtual void runSimulation()=0;
        virtual void printSimulationResults()=0;
};

// WiFi Simulation Class
// class WiFi4Simulation : public WiFiSimulation {
// private:
//     AccessPoint m_accessPoint;
//     std::vector<User> m_users;

// public:
//     WiFi4Simulation(size_t userCount, const std::string& apId = "AP1");

//     void runSimulation();
//     void printSimulationResults();
// };

class WiFi4Simulation {
protected:
    // Make m_users protected to allow access in derived classes
    std::vector<User> m_users;
    AccessPoint m_accessPoint;

public:
    WiFi4Simulation(size_t userCount, const std::string& apId = "AP1");

    // Getter for users
    std::vector<User>& getUsers() { return m_users; }
    const std::vector<User>& getUsers() const { return m_users; }

    virtual void runSimulation();
    virtual void printSimulationResults();
};
#endif // WIFI4_SIMULATION_H
