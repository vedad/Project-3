#include <armadillo>

using namespace std;
using namespace arma;

class CelestialObject {

	public:
		CelestialObject(string, vec, vec, double);
		string name;
		vec velocity, position;
		double mass;

		// Set-functions
		setVelocity(vec);
		setPosition(vec);
		
		// Get-functions
		getDistanceTo(CelestialObject);
		getForce(CelestialObject);
		getAcceleration(CelestialObject);
		getVelocity();
		getPosition();
		getName();
		getMass();

		
};

