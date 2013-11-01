#include <iostream>
#include "SolarSystem.hpp"
#include <ctime>

using namespace std;

int main(int argc, char* argv[]) {
	
	clock_t start, finish;	
	start = clock();
	SolarSystem mySystem = SolarSystem("../data/parameters/sunEarthJupiter.dat");
	mySystem.systemSimulation(1e-1, 100, false, false);
	finish = clock();
	cout << "Computation time: " << double(finish - start)/CLOCKS_PER_SEC << " seconds" << endl;

	return 0;
}


