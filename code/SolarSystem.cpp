#include <iostream>
#include <armadillo>
#include <fstream>
#include <sstream>
#include <vector>
#include "SolarSystem.hpp"

using namespace std;
using namespace arma;

const double DIMENSION = 2;
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
		cout << "Added: " << newObject.getName() << endl;

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
	mat accK1 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat accK2 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat accK3 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat accK4 = zeros<mat>(DIMENSION, getNoOfObjects());

	mat velK1 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat velK2 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat velK3 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat velK4 = zeros<mat>(DIMENSION, getNoOfObjects());

	mat posK1 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat posK2 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat posK3 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat posK4 = zeros<mat>(DIMENSION, getNoOfObjects());

	mat newVelocity = zeros<mat>(DIMENSION, getNoOfObjects());
	mat newPosition = zeros<mat>(DIMENSION, getNoOfObjects());

	double dt2 = dt * 0.5;
	double dt6 = dt / 6.0;
	
	// Get acceleration, velocity and position in the first step, K1.
	for (int i=0; i < getNoOfObjects(); i++) {
		velK1.col(i) = objects[i].getVelocity();
		posK1.col(i) = objects[i].getPosition();
		accK1.col(i) = getSystemAcceleration(objects[i]);


	}
	
	
	// Calculate acceleration, velocity and position in the second step, K2.
	for (int i=0; i < getNoOfObjects(); i++) {

		velK2.col(i) = velK1.col(i) + dt2 * accK1.col(i);
		posK2.col(i) = posK1.col(i) + dt2 * velK1.col(i);
		objects[i].setVelocity(velK2.col(i));
		objects[i].setPosition(posK2.col(i));
		accK2.col(i) = getSystemAcceleration(objects[i]);

	}
	

	// Calculate acceleration, velocity and position in the third step, K3.
	for (int i=0; i < getNoOfObjects(); i++) {

		velK3.col(i) = velK2.col(i) + dt2 * accK2.col(i);
		posK3.col(i) = posK2.col(i) + dt2 * velK2.col(i);
		objects[i].setVelocity(velK3.col(i));
		objects[i].setPosition(posK3.col(i));
		accK3.col(i) = getSystemAcceleration(objects[i]);

	}

	
	// Calculate acceleration, velocity and position in the fourth step, K4.
	for (int i=0; i < getNoOfObjects(); i++) {

		velK4.col(i) = velK3.col(i) + dt * accK3.col(i);
		posK4.col(i) = posK3.col(i) + dt * velK3.col(i);
		objects[i].setVelocity(velK4.col(i));
		objects[i].setPosition(posK4.col(i));
		accK4.col(i) = getSystemAcceleration(objects[i]);

	}

	
	// Calculate the new acceleration, velocity and position after a time step
	// dt.
	for (int i=0; i < getNoOfObjects(); i++) {

		newVelocity.col(i) = velK1.col(i) + dt6 * (accK1.col(i) + 2*accK2.col(i) + 2*accK3.col(i) + accK4.col(i));	

		newPosition.col(i) = posK1.col(i) + dt6 * (velK1.col(i) + 2*velK2.col(i) + 2*velK3.col(i) + velK4.col(i));

		objects[i].setVelocity(newVelocity.col(i));
		objects[i].setPosition(newPosition.col(i));
	}


}

void SolarSystem :: systemSimulation(double dt, double tMax) {
	
//	fstream outFile;
//	outFile.open("../data/simulationData.dat", ios::out);
	ofstream *newFile;
	vector<ofstream*> objectFileList;

	for (int i=0; i < getNoOfObjects(); i++) {

				
		ostringstream objectFile;
		objectFile << "../data/objects/" << objects[i].getName() << ".dat";
		newFile = new ofstream(objectFile.str().c_str());
		*newFile << "Positions for: " << objects[i].getName() << endl;
		objectFileList.push_back(newFile);
	}

	cout << "Advancing system..." << endl;

	for (double t=0; t < tMax; t+=dt) {
		
		this->advance(dt);
		
		for (int i=0; i < getNoOfObjects(); i++) {
			
			*objectFileList[i] << objects[i].getPosition()[0] << " " << objects[i].getPosition()[1] << endl;
		}
	}

	for (int i = 0; i < getNoOfObjects(); i++) {
		objectFileList[i]->close();
	}
//	outFile.close();
}

// Calculating the force from all celestial objects on an object
vec SolarSystem :: getSystemForce(CelestialObject object) {
	
	// Initialize systemForce as a vector of size 2, with zeros as entries since
	// I use += function below.
	vec systemForce = zeros<vec>(DIMENSION);
	for (int i=0; i < getNoOfObjects(); i++) {
	
		if (object.getName() == objects[i].getName()) { continue; }

		else {

			vec r = object.getDistanceTo(objects[i]);
			systemForce += object.getForce(objects[i]);
		}
	}

	return systemForce;
}

// Calculating the acceleration of an object
vec SolarSystem :: getSystemAcceleration(CelestialObject object) {
	
	vec systemForce = getSystemForce(object);
	vec systemAcceleration = systemForce / object.getMass();

	return systemAcceleration;
}

int SolarSystem :: getNoOfObjects() { return objects.size(); }


