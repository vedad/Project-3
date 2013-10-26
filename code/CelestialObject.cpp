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
// Set-functions
void CelestialObject :: setVelocity(vec newVelocity) {

	velocity = newVelocity;

}

void CelestialObject :: setPosition(vec newPosition) {

	position = newPosition;

}

// Get-functions
vec CelestialObject :: getDistanceTo(CelestialObject other) {
	
	vec distance = other.getPosition() - this->getPosition();
	return distance;

}

vec CelestialObject :: getForce(CelestialObject other) {
	
	vec r = this->getDistanceTo(other);
	vec force = (this->getMass() * other.getMass() / pow(norm(r,2),3)) * r;
	return force;
		
}

vec CelestialObject :: getAcceleration(CelestialObject other) {

	vec force = getForce(other);
	vec acceleration = force/(this->getMass());
	return acceleration;

}

vec CelestialObject :: getVelocity() { return velocity; }
vec CelestialObject :: getPosition() { return position; }
double CelestialObject :: getMass() { return mass; }
string CelestialObject :: getName() { return name; }

