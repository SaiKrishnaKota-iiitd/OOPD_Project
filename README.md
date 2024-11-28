# README: WiFi Communication Simulation
    Overview
        This project simulates the behavior of WiFi communication across three major standards: WiFi 4, WiFi 5, and WiFi 6, using C++. Each simulation models the transmission process for different numbers of users and calculates the throughput, average latency, and maximum latency.

    The implementation focuses on designing a robust object-oriented structure to accurately represent the real-world elements of WiFi communication, including Access Points (APs), Users, Packets, and Frequency Channels.

    Problem Breakdown
    WiFi 4 Simulation
        WiFi 4 operates with a single access point and multiple users. The transmission involves:

            Sniffing the channel.
            Transmitting data if the channel is free.
            Deferring by a random time if the channel is busy, then retrying.
        Objective:

        Calculate throughput, average latency, and maximum latency for:
            1 user and 1 AP
            10 users and 1 AP
            100 users and 1 AP
            
    WiFi 5 Simulation
        WiFi 5 introduces multi-user MIMO, enabling parallel communication after:

            A broadcast packet from the AP.
            Channel state information packets sent sequentially by users.
            Parallel communication for 15ms.
        Objective:

        Compute throughput, average latency, and maximum latency for the same three scenarios:
            1 user and 1 AP
            10 users and 1 AP
            100 users and 1 AP

    WiFi 6 Simulation
        WiFi 6 supports OFDMA (Orthogonal Frequency-Division Multiple Access), allowing:

            Division of a 20 MHz channel into smaller sub-channels (2 MHz, 4 MHz, 10 MHz).
            Parallel use of sub-channels for 5ms.
        Objective:

            Calculate throughput, average latency, and maximum latency under similar conditions as WiFi 4 and WiFi 5.
    
    Key Features of Implementation
    Object-Oriented Design
        Class Structure:
            AccessPoint: Represents the AP managing users and channels.
            User: Models individual users transmitting data packets.
            Packet: Encapsulates data size and transmission metadata.
            FrequencyChannel: Represents the available bandwidth and sub-channels.
        Inheritance and Polymorphism:
            Shared base classes for common behaviors and specialized classes for unique features of WiFi 4, WiFi 5, and WiFi 6.
        Encapsulation:
            Use of private and protected members for secure data handling.
            Templates and Exception Handling
            Templates for generalized handling of packet queues and user data.
            Exception Handling for errors such as invalid bandwidth allocation or channel congestion.
        Data Structures
            Queues for packet scheduling.
            Vectors for managing users dynamically.
            Results and Metrics
    
    For each scenario, the simulation outputs:
        Throughput: Total data transmitted successfully.
        Average Latency: Mean time taken for packet delivery.
        Maximum Latency: Longest time experienced by any packet.
    
    AI in Code Development
        Artificial Intelligence (AI) tools were utilized during the development process to:
        Generate Initial Code: AI-assisted creation of the class hierarchy and function prototypes.
        Refine Algorithms: Iterative improvements in logic to optimize performance.
        Debugging and Testing: AI-driven suggestions for resolving runtime and logical errors.
        Documentation: Automating parts of this README to improve clarity and reduce manual effort.


# Commands to build the library

	g++ -std=c++17 -fPIC -c WiFiSimulation.cpp -o impl1.o
	g++ -std=c++17 -fPIC -c wifi5_simulation.cpp -o impl2.o
	g++ -std=c++17 -fPIC -c wifi6_simulation.cpp -o impl3.o
	g++ -shared -o libmylibrary.so impl1.o impl2.o impl3.o

# commands to test the library
    g++ -std=c++17 -fPIC -g WiFiSimulation.cpp wifi5_simulation.cpp wifi4_main.cpp -o wifi4_sim_debug -L. -lmylibrary

    g++ -std=c++17 -fPIC -g WiFiSimulation.cpp wifi5_simulation.cpp wifi5_main.cpp -o wifi5_sim_debug -L. -lmylibrary

    g++ -std=c++17 -fPIC -g  WiFiSimulation.cpp wifi5_simulation.cpp wifi6_simulation.cpp wifi6_main.cpp -o wifi6_sim_debug -L. -lmylibrary

