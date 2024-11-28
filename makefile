compile:
	g++ -std=c++17 WiFiSimulation.cpp wifi4_main.cpp -o wifi4_sim

simulate5:
	g++ -std=c++17 WiFiSimulation.cpp wifi5_simulation.cpp wifi5_main.cpp -o wifi5_sim

simulate6:
	g++ -std=c++17 WiFiSimulation.cpp wifi5_simulation.cpp wifi6_simulation.cpp wifi6_main.cpp -o wifi6_sim
