#include <iostream>
#include <cmath>
#include <armadillo>
#include "CelestialObject.hpp"

CelestialObject::CelestialObject(double mass) {
	m = mass;	
}

double CelestialObject::getMass() {

	return m;
}
