#include <iostream>
#include "CelestialObject.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	
	CelestialObject Earth = CelestialObject(100);
	
	cout << Earth.getMass() << endl;

	return 0;
}


