#include "Physics.hpp"


Physics::Physics(void)
{
}


Physics::~Physics(void)
{
}

bool Physics::KollisionKameraObjektVorhanden(vector<vector<float>> objekte, float x, float y, float z)
{
	bool collsion = false;
	for (int i = 0; i < objekte.size(); i++)
	{
		if (
			((objekte.at(i)[0] <= x + 0.5 && x - 0.5 <= objekte.at(i)[3]) ||
				(objekte.at(i)[0] <= x + 0.5 && x - 0.5 <= objekte.at(i)[3]))
			&&
			((objekte.at(i)[1] <= y + 0.5 && y - 0.5 <= objekte.at(i)[4]) ||
				(objekte.at(i)[1] <= y + 0.5 && y - 0.5 <= objekte.at(i)[4]))
			&&
			((objekte.at(i)[2] <= z + 0.5 && z - 0.5 <= objekte.at(i)[5]) ||
				(objekte.at(i)[2] <= z + 0.5 && z - 0.5 <= objekte.at(i)[5]))

		)
		{
			//cout << objekte.at(1).[0] <<"<="<< objekte.at(0).[3]<< " " <<objekte.at(0).[0] <<"<=" <<objekte.at(1).[3] << "\n";
			collsion = true;
			//cout << "Collision Tür \n";
			break;
		}
		else
		{
			collsion = false;
		}
	}
	return collsion;
}


int Physics::KollisionKameraObjekt(vector<vector<float>> objekte, float x, float y, float z)
{
	int collsion = 99;
	for (int i = 0; i < objekte.size(); i++)
	{
		if (
			((objekte.at(i)[0] <= x + 0.5 && x - 0.5 <= objekte.at(i)[3]) ||
				(objekte.at(i)[0] <= x + 0.5 && x - 0.5 <= objekte.at(i)[3]))
			&&
			((objekte.at(i)[1] <= y + 0.5 && y - 0.5 <= objekte.at(i)[4]) ||
				(objekte.at(i)[1] <= y + 0.5 && y - 0.5 <= objekte.at(i)[4]))
			&&
			((objekte.at(i)[2] <= z + 0.5 && z - 0.5 <= objekte.at(i)[5]) ||
				(objekte.at(i)[2] <= z + 0.5 && z - 0.5 <= objekte.at(i)[5]))

		)
		{
			//cout << objekte.at(1).[0] <<"<="<< objekte.at(0).[3]<< " " <<objekte.at(0).[0] <<"<=" <<objekte.at(1).[3] << "\n";
			collsion = i;
			//cout << "Collision Tür \n";
			break;
		}
	}
	return collsion;
}


bool Physics::Kollision3DKamera(vector<vector<float>> objekte, float x, float y, float z)
{
	bool collsion = false;
	for (int i = 0; i < objekte.size(); i++)
	{
		if (
			((objekte.at(i)[0] <= x + 0.5 && x - 0.5 <= objekte.at(i)[3]) ||
				(objekte.at(i)[0] <= x + 0.5 && x - 0.5 <= objekte.at(i)[3]))
			&&
			((objekte.at(i)[1] <= y + 0.5 && y - 0.5 <= objekte.at(i)[4]) ||
				(objekte.at(i)[1] <= y + 0.5 && y - 0.5 <= objekte.at(i)[4]))
			&&
			((objekte.at(i)[2] <= z + 0.5 && z - 0.5 <= objekte.at(i)[5]) ||
				(objekte.at(i)[2] <= z + 0.5 && z - 0.5 <= objekte.at(i)[5]))

		)
		{
			//cout << objekte.at(1).[0] <<"<="<< objekte.at(0).[3]<< " " <<objekte.at(0).[0] <<"<=" <<objekte.at(1).[3] << "\n";
			collsion = true;
			//cout << "Collision \n";
			break;
		}
		else
		{
			collsion = false;
		}
	}
	return collsion;
}

bool Physics::positionsAbfrage(float xZiel, float zZiel, float x, float y, float z)
{
	bool isPosition = false;
	if (
		(xZiel - 4 <= x && x <= xZiel + 4)
		&&
		(zZiel - 4 <= z && z <= zZiel + 4)

	)
	{
		isPosition = true;
		//cout << "Fertig \n";
	}
	else
	{
		isPosition = false;
	}


	return isPosition;
}
