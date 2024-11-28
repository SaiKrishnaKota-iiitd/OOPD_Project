libmylibrary.so: impl1.o impl2.o impl3.o
	g++ -shared -o libmylibrary.so impl1.o impl2.o impl3.o

impl1.o: WiFiSimulation.cpp
	g++ -std=c++17 -fPIC -c WiFiSimulation.cpp -o impl1.o

impl2.o: wifi5_simulation.cpp
	g++ -std=c++17 -fPIC -c wifi5_simulation.cpp -o impl2.o

impl3.o: wifi6_simulation.cpp
	g++ -std=c++17 -fPIC -c wifi6_simulation.cpp -o impl3.o


# Simulate 5 (Linking with the shared library)
simulate4: WiFiSimulation.cpp wifi5_simulation.cpp wifi4_main.cpp
	g++ -std=c++17 -fPIC -g WiFiSimulation.cpp wifi5_simulation.cpp wifi4_main.cpp -o wifi4_sim_debug -L. -lmylibrary
	g++ -std=c++17 -fPIC -O3 WiFiSimulation.cpp wifi5_simulation.cpp wifi4_main.cpp -o wifi4_sim_opt -L. -lmylibrary

simulate5: WiFiSimulation.cpp wifi5_simulation.cpp wifi5_main.cpp
	g++ -std=c++17 -fPIC -g WiFiSimulation.cpp wifi5_simulation.cpp wifi5_main.cpp -o wifi5_sim_debug -L. -lmylibrary
	g++ -std=c++17 -fPIC -O3 WiFiSimulation.cpp wifi5_simulation.cpp wifi5_main.cpp -o wifi5_sim_opt -L. -lmylibrary

# Simulate 6 (Linking with the shared library)
simulate6: WiFiSimulation.cpp wifi5_simulation.cpp wifi6_simulation.cpp wifi6_main.cpp
	g++ -std=c++17 -fPIC -g  WiFiSimulation.cpp wifi5_simulation.cpp wifi6_simulation.cpp wifi6_main.cpp -o wifi6_sim_debug -L. -lmylibrary
	g++ -std=c++17 -fPIC -O3 WiFiSimulation.cpp wifi5_simulation.cpp wifi6_simulation.cpp wifi6_main.cpp -o wifi6_sim_opt -L. -lmylibrary
	./wifi6_sim

# Clean up object files and shared library
clean:
	rm -f *.o libmylibrary.so wifi5_sim_opt wifi5_sim_debug wifi6_sim_opt wifi6_sim_debug wifi4_sim_opt wifi4_sim_debug
