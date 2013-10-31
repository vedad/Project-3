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
		void systemSimulation(double, double, bool, bool);
		double getSystemPotentialEnergy(CelestialObject);
		double getTotalEnergy(CelestialObject);
		double getAngularMomentum(CelestialObject);
		int getNoOfObjects();
	
		vector<CelestialObject> objects;

};
