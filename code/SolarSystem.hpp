#include <vector>
#include "CelestialObject.hpp"

using namespace std;

class SolarSystem {
	
	public:
		SolarSystem(string);
		void addObject(CelestialObject);
		void advance();
		int getNoOfObjects();
	
		vector<CelestialObject> objects;

};
