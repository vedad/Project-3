#include <iostream>
#include "SolarSystem.hpp"
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
	
//	CelestialObject Earth = CelestialObject(100);
//	cout << Earth.getMass() << endl;
	
	clock_t start, finish;	
	start = clock();
	SolarSystem mySystem = SolarSystem("../data/sunEarthSystem.dat");
	mySystem.systemSimulation(1e-4, 3);
	finish = clock();
	cout << "Computation time: " << double(finish - start)/CLOCKS_PER_SEC << " seconds" << endl;

	return 0;
}


