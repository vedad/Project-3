#include <vector>
#include "CelestialObject.hpp"

using namespace std;

class SolarSystem {
	
	public:
		SolarSystem(string);
		vec getSystemForce(CelestialObject);
		vec getSystemAcceleration(CelestialObject);
		void addObject(CelestialObject);
		void advance();
		int getNoOfObjects();
	
		vector<CelestialObject> objects;

};
