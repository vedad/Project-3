#include <iostream>
#include <armadillo>
#include <fstream>
#include "SolarSystem.hpp"

using namespace std;

SolarSystem :: SolarSystem(string systemfile) {

	fstream inFile;
	inFile.open("../data/sunEarthSystem.dat", ios::in);
	
	double x0, y0, v0x, v0y, m;
	string name;

	while (inFile >> name >> x0 >> y0 >> v0x >> v0y >> m) {
		
		vec position; position << x0 << y0;
		vec velocity; velocity << v0x << v0y;
		CelestialObject newObject = CelestialObject(name, position, velocity, m);
		addObject(newObject);

	}
}

void SolarSystem :: addObject(CelestialObject newObject) {
	objects.push_back(newObject);
}



void SolarSystem :: advance() {


}
