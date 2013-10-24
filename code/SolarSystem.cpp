#include <iostream>
#include <armadillo>
#include <fstream>
#include "SolarSystem.hpp"

using namespace std;

const double DIMENSIONALITY = 2;
const double GRAV_CONST = 6.67384e-11; // [m^3 kg^(-1) s^(-2)]
/* Need to change gravitational constant so that it can be used with AU as the
 * unit of length, and years as the unit of time */

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


/* Advancing the system a time step dt */

void SolarSystem :: advance(double dt) {

	/* Need each of the K's to be a matrix, where the rows represent each
	 * object, and the first and second columns giving the contribution in x and
	 * y direction. */	

	mat accK1 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat accK2 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat accK3 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat accK4 = zeros<mat>(getNoOfObjects(), DIMENSION);

	mat velK1 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat velK2 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat velK3 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat velK4 = zeros<mat>(getNoOfObjects(), DIMENSION);

	mat posK1 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat posK2 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat posK3 = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat posK4 = zeros<mat>(getNoOfObjects(), DIMENSION);

	mat newVelocity = zeros<mat>(getNoOfObjects(), DIMENSION);
	mat newPosition = zeros<mat>(getNoOfObjects(), DIMENSION);

	dt2 = dt * 0.5;
	dt6 = dt / 6.0;
	
	// Get acceleration, velocity and position in the first step.
	for (int i=0; i < getNoOfObjects(); i++) {

		accK1.row(i) = objects[i].getAcceleration();
		velK1.row(i) = objects[i].getVelocity();
		posK1.row(i) = objects[i].getPosition();

	}
	
	// Calculate acceleration, velocity and position in the second step.
	for (int i=0; i < getNoOfObjects(); i++) {

		velK2.row(i) = velK1.row(i) + dt2 * accK1.row(i);
		posK2.row(i) = posK1.row(i) + dt2 * velK1.row(i);
		objects[i].setPosition(posK2.row(i));
		accK2.row(i) = objects[i].getAcceleration();

	}
	
	// Calculate acceleration, velocity and position in the third step.
	for (int i=0; i < getNoOfObjects(); i++) {

		velK3.row(i) = velK2.row(i) + dt2 * accK2.row(i);
		posK3.row(i) = posK2.row(i) + dt2 * velK2.row(i);
		objects[i].setPosition(posK3.row(i));
		accK3.row(i) = objects[i].getAcceleration();

	}
	
	// Calculate acceleration, velocity and position in the fourth step.
	for (int i=0; i < getNoOfObjects(); i++) {

		velK4.row(i) = velK3.row(i) + dt * accK3.row(i);
		posK4.row(i) = posK3.row(i) + dt * velK3.row(i);
		objects[i].setPosition(posK4.row(i));
		accK4.row(i) = objects[i].getAcceleration();

	}
	
	// Calculate the new acceleration, velocity and position after a time step
	// dt.
	for (int i=0; i < getNoOfObjects(); i++) {

		newVelocity(i) = velK1.row(i) + dt6 * (accK1.row(i) + 2*accK2.row(i) + 2*accK3.row(i) + accK4.row(i));	

		newPosition(i) = posK1.row(i) + dt6 * (velK1.row(i) + 2*velK2.row(i) + 2*velK3.row(i) + velK4.row(i));

		objects[i].setVelocity(newVelocity(i));
		objects[i].setPosition(newPosition(i));
	}

}

int SolarSystem :: getNoOfObjects() { return objects.size(); }
