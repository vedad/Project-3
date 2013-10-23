#include <iostream>
#include <armadillo>
#include "CelestialObject.hpp"

using namespace std;
using namespace arma;

CelestialObject :: CelestialObject(string name, vec position, vec velocity, double mass) {

	this->mass = mass;
	this->position = position;
	this->velocity = velocity;
	this->name = name;

}



