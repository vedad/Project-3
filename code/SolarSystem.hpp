#include <vector>
#include "CelestialObject.hpp"

using namespace std;
using namespace arma;

class SolarSystem {
	
	public:
		SolarSystem(string);
		vec getSystemForce(CelestialObject);
		vec getSystemAcceleration(CelestialObject);
		void addObject(CelestialObject);
		void advance(double);
		void systemSimulation(double, double);
		int getNoOfObjects();
	
		vector<CelestialObject> objects;

};
