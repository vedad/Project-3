#include <iostream>
#include "SolarSystem.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	
//	CelestialObject Earth = CelestialObject(100);
//	cout << Earth.getMass() << endl;

	SolarSystem mySystem = SolarSystem("../data/sunEarthSystem.dat");

	return 0;
}


