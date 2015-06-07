#pragma once
#include "Objekt.hpp"
#include <iostream>
using namespace std;

class Physics
{
public:
	Physics(void);
	~Physics(void);

	bool Kollision3DKamera(vector<vector<float>> objekte, float x, float y, float z);
	int KollisionKameraObjekt(vector<vector<float>> objekte, float x, float y, float z);
	bool KollisionKameraObjektVorhanden(vector<vector<float>> objekte, float x, float y, float z);

	bool positionsAbfrage(float xziel, float zziel, float x, float y, float z);
};
