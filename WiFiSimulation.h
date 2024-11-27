#ifndef WIFI_SIMULATION_H
#define WIFI_SIMULATION_H

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <numeric>
#include <limits>

// Base frequency class
class BaseFrequency {
public:
    virtual double getBandwidth() const = 0;
    virtual ~BaseFrequency() = default;
};

// Template class for specific frequency
template <typename T>
class Frequency : public BaseFrequency {
    T bandwidth;

public:
    explicit Frequency(T bw) : bandwidth(bw) {}
    double getBandwidth() const override { return static_cast<double>(bandwidth); }
};

// Packet class
class Packet {
public:
    int size; // in bytes
    double transmissionTime; // Time taken to transmit this packet

    Packet(int s, double time) : size(s), transmissionTime(time) {}
};

// User class
class User {
    int id;
    double backoffTime;
    std::queue<Packet> packetQueue;

public:
    explicit User(int userId) : id(userId), backoffTime(0) {}

    void generatePacket(BaseFrequency* frequency, double modulationRate, double codingRate);
    Packet getNextPacket();
    bool hasPackets() const;
    double getBackoffTime() const;
    void setBackoffTime(double time);
};

// Access Point class
class AccessPoint {
    std::vector<User*> users;
    bool channelFree;

public:
    explicit AccessPoint() : channelFree(true) {}
    void connectUser(User* user);
    bool isChannelFree() const;
    void setChannelFree(bool status);
    void simulateTransmission(double& throughput, double& avgLatency, double& maxLatency);
};

// Simulation Utility
class WiFiSimulation {
    BaseFrequency* frequency;
    AccessPoint accessPoint;
    int numUsers;

public:
    WiFiSimulation(BaseFrequency* freq, int users)
        : frequency(freq), numUsers(users) {}

    void setupSimulation();
    void runSimulation();
};

#endif // WIFI_SIMULATION_H
