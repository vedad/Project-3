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

	mat velK1 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat velK2 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat velK3 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat velK4 = zeros<mat>(DIMENSION, getNoOfObjects());

	mat posK1 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat posK2 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat posK3 = zeros<mat>(DIMENSION, getNoOfObjects());
	mat posK4 = zeros<mat>(DIMENSION, getNoOfObjects());

	mat thisVelocity = zeros<mat>(DIMENSION, getNoOfObjects());
	mat thisPosition = zeros<mat>(DIMENSION, getNoOfObjects());

	mat newVelocity = zeros<mat>(DIMENSION, getNoOfObjects());
	mat newPosition = zeros<mat>(DIMENSION, getNoOfObjects());

	double dt2 = dt / 2.0;
	double dt6 = dt / 6.0;
		
	for (int i=0; i < getNoOfObjects(); i++) {
		thisPosition.col(i) = objects[i].getPosition();
		thisVelocity.col(i) = objects[i].getVelocity();

		velK1.col(i) = getSystemAcceleration(objects[i]);
		posK1.col(i) = objects[i].getVelocity();			
	}
	
	// Move system by half the step length and calculate K2.
	for (int i=0; i < getNoOfObjects(); i++) {
		objects[i].setPosition(thisPosition.col(i) + dt2 * posK1.col(i));
		velK2.col(i) = getSystemAcceleration(objects[i]);
		posK2.col(i) = posK1.col(i) + dt2 * velK1.col(i);
	}
	
	// Move system by half a step length and calculate K3.
	for (int i=0; i < getNoOfObjects(); i++) {
		objects[i].setPosition(thisPosition.col(i) + dt2 * posK2.col(i));
		velK3.col(i) = getSystemAcceleration(objects[i]);
		posK3.col(i) = posK1.col(i) + dt2 * velK2.col(i);
	}
	
	// Move system by half a step length and calculate K4.
	for (int i=0; i < getNoOfObjects(); i++) {
		objects[i].setPosition(thisPosition.col(i) + dt*posK3.col(i));
		velK4.col(i) = getSystemAcceleration(objects[i]);
		posK4.col(i) = posK1.col(i) + dt * velK3.col(i);
	}
	
	// Calculate the new velocity and position after a time step dt.
	for (int i=0; i < getNoOfObjects(); i++) {
		newVelocity.col(i) = thisVelocity.col(i) + dt6 * (velK1.col(i) + 2*velK2.col(i) + 2*velK3.col(i) + velK4.col(i));	

		newPosition.col(i) = thisPosition.col(i) + dt6 * (posK1.col(i) + 2*posK2.col(i) + 2*posK3.col(i) + posK4.col(i));

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


