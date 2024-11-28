#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <cmath>
#include <iomanip>
#include <algorithm>
// #include <thread>
#include <mutex>
#include <chrono>

class Packet {
public:
    int size;              // Size in bytes
    double transmissionTime; // Transmission time in ms
    double latency;          // Latency in ms

    Packet(int size, double time) : size(size), transmissionTime(time), latency(0) {}
};

class User {
public:
    int id;
    double backoffTime;
    std::queue<Packet> packetQueue;
    double totalLatency;

    User(int userId) : id(userId), backoffTime(0), totalLatency(0) {}

    void generatePackets(int numPackets, int packetSize, double transmissionRate) {
        for (int i = 0; i < numPackets; ++i) {
            double txTime = (packetSize * 8) / transmissionRate; // in ms
            packetQueue.emplace(packetSize, txTime);
        }
    }
};

class Frequency {
public:
    double totalBandwidth;        // Total bandwidth in MHz
    std::vector<double> subChannels; // Subdivided channels for OFDMA

    Frequency(double bandwidth) : totalBandwidth(bandwidth) {}

    void subdivide(double subChannelWidth) {
        int numChannels = static_cast<int>(totalBandwidth / subChannelWidth);
        subChannels.clear();
        for (int i = 0; i < numChannels; ++i) {
            subChannels.push_back(subChannelWidth);
        }
    }

    void clearSubdivisions() {
        subChannels.clear();
    }
};


class AccessPoint {
public:
    bool channelFree;
    double bandwidth; // in Hz
    int users;
    std::vector<User*> userList;

    AccessPoint(double bw) : channelFree(true), bandwidth(bw), users(0) {}

    void addUser(User* user) {
        userList.push_back(user);
        ++users;
    }
};

class WiFiSimulator {
protected:
    AccessPoint* ap;
    int packetSize;
    double bandwidth;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> backoffDist;

    WiFiSimulator(double bw, int packetSz)
        : bandwidth(bw), packetSize(packetSz), backoffDist(1, 10) {
        ap = new AccessPoint(bw);
    }

public:
    virtual void simulate(int numUsers, int packetsPerUser) = 0;
    virtual ~WiFiSimulator() { delete ap; }
};

class WiFi4Simulator : public WiFiSimulator {
public:
    WiFi4Simulator(double bw, int packetSz)
        : WiFiSimulator(bw, packetSz) {}

    void simulate(int numUsers, int packetsPerUser) override {
        std::cout << "Simulating WiFi 4...\n";
        ap->userList.clear();

        // Add users and generate packets
        for (int i = 0; i < numUsers; ++i) {
            User* user = new User(i);
            user->generatePackets(packetsPerUser, packetSize, bandwidth);
            ap->addUser(user);
        }

        double totalThroughput = 0;
        double totalLatency = 0;
        double maxLatency = 0;
        int transmittedPackets = 0;

        // Simulation Loop
        for (auto& user : ap->userList) {
            while (!user->packetQueue.empty()) {
                if (ap->channelFree) {
                    ap->channelFree = false;

                    Packet packet = user->packetQueue.front();
                    user->packetQueue.pop();

                    totalThroughput += packet.size; // Add size in bytes
                    user->totalLatency += packet.transmissionTime;
                    maxLatency = std::max(maxLatency, user->totalLatency);

                    // Simulate transmission
                    // std::this_thread::sleep_for(std::chrono::milliseconds(
                    //     static_cast<int>(packet.transmissionTime)));
                    ap->channelFree = true;

                    transmittedPackets++;
                } else {
                    // Backoff
                    double backoff = backoffDist(generator);
                    user->totalLatency += backoff;
                    // std::this_thread::sleep_for(std::chrono::milliseconds(
                    //     static_cast<int>(backoff)));
                }
            }
            totalLatency += user->totalLatency;
        }

        // Results
        double avgLatency = totalLatency / transmittedPackets;
        double throughputMbps = (totalThroughput * 8) / (1e6); // Bytes to Mbps

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Throughput: " << throughputMbps << " Mbps\n";
        std::cout << "Average Latency: " << avgLatency << " ms\n";
        std::cout << "Max Latency: " << maxLatency << " ms\n";

        // Clean up users
        for (auto& user : ap->userList) {
            delete user;
        }
    }
};

class WiFi5Simulator : public WiFiSimulator {
public:
    WiFi5Simulator(double bw, int packetSz)
        : WiFiSimulator(bw, packetSz) {}

    void simulate(int numUsers, int packetsPerUser) override {
        std::cout << "Simulating WiFi 5...\n";
        ap->userList.clear();

        // Add users and generate packets
        for (int i = 0; i < numUsers; ++i) {
            User* user = new User(i);
            user->generatePackets(packetsPerUser, packetSize, bandwidth);
            ap->addUser(user);
        }

        double totalThroughput = 0;
        double totalLatency = 0;
        double maxLatency = 0;
        int transmittedPackets = 0;

        // Simulation Loop
        while (true) {
            // CSI Exchange
            double csiTime = 0;
            for (auto& user : ap->userList) {
                csiTime += (200 * 8) / bandwidth; // Transmission time for 200 bytes CSI
            }

            // Parallel Transmission
            for (auto& user : ap->userList) {
                int packetsToTransmit = std::min(packetsPerUser, 15); // 15 ms window
                for (int i = 0; i < packetsToTransmit && !user->packetQueue.empty(); ++i) {
                    Packet packet = user->packetQueue.front();
                    user->packetQueue.pop();

                    totalThroughput += packet.size; // Bytes
                    user->totalLatency += csiTime + packet.transmissionTime;
                    maxLatency = std::max(maxLatency, user->totalLatency);

                    transmittedPackets++;
                }
            }

            // Exit condition (all users' queues are empty)
            if (std::all_of(ap->userList.begin(), ap->userList.end(),
                            [](User* user) { return user->packetQueue.empty(); })) {
                break;
            }
        }

        // Results
        double avgLatency = totalLatency / transmittedPackets;
        double throughputMbps = (totalThroughput * 8) / (1e6); // Bytes to Mbps

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Throughput: " << throughputMbps << " Mbps\n";
        std::cout << "Average Latency: " << avgLatency << " ms\n";
        std::cout << "Max Latency: " << maxLatency << " ms\n";

        // Clean up users
        for (auto& user : ap->userList) {
            delete user;
        }
    }
};


class WiFi6Simulator : public WiFiSimulator {
private:
    Frequency* frequency;

public:
    WiFi6Simulator(double bw, int packetSz)
        : WiFiSimulator(bw, packetSz) {
        frequency = new Frequency(bw);
    }

    ~WiFi6Simulator() { delete frequency; }

    void simulate(int numUsers, int packetsPerUser) override {
        std::cout << "Simulating WiFi 6...\n";
        ap->userList.clear();

        // Add users and generate packets
        for (int i = 0; i < numUsers; ++i) {
            User* user = new User(i);
            user->generatePackets(packetsPerUser, packetSize, bandwidth);
            ap->addUser(user);
        }

        // Subdivide channels
        frequency->subdivide(2.0); // Example: 2 MHz sub-channels

        double totalThroughput = 0;
        double totalLatency = 0;
        double maxLatency = 0;
        int transmittedPackets = 0;

        // Simulation Loop
        while (true) {
            // Allocate sub-channels to users
            for (size_t i = 0; i < ap->userList.size(); ++i) {
                if (i >= frequency->subChannels.size()) break; // No more sub-channels
                User* user = ap->userList[i];

                int packetsToTransmit = std::min(packetsPerUser, 5); // 5 ms window
                for (int j = 0; j < packetsToTransmit && !user->packetQueue.empty(); ++j) {
                    Packet packet = user->packetQueue.front();
                    user->packetQueue.pop();

                    totalThroughput += packet.size; // Bytes
                    user->totalLatency += packet.transmissionTime;
                    maxLatency = std::max(maxLatency, user->totalLatency);

                    transmittedPackets++;
                }
            }

            // Exit condition (all users' queues are empty)
            if (std::all_of(ap->userList.begin(), ap->userList.end(),
                            [](User* user) { return user->packetQueue.empty(); })) {
                break;
            }
        }

        // Results
        double avgLatency = totalLatency / transmittedPackets;
        double throughputMbps = (totalThroughput * 8) / (1e6); // Bytes to Mbps

        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Throughput: " << throughputMbps << " Mbps\n";
        std::cout << "Average Latency: " << avgLatency << " ms\n";
        std::cout << "Max Latency: " << maxLatency << " ms\n";

        // Clean up users
        for (auto& user : ap->userList) {
            delete user;
        }
    }
};
