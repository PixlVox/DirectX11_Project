#pragma once
#include"Geometry.h"
#include "Terrain.h"
#include <vector>

/*
Denna klass �r t�nkt att hantera de olika Object och ljusk�llor i scenen
i form av beh�llare, objekten kallar sin draw() funktion och kan �ndra sitt inneh�ll mm

*/

class GeometryHandler
{

private:
	std::vector<Geometry*> GeometryObjects; //holds Geometry objects
	Terrain* terrainObj;
	RenderEngine* engine;
public:
	GeometryHandler(RenderEngine* engine);
	~GeometryHandler();

	void update();
};