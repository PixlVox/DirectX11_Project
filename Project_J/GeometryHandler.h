#pragma once
#include"Drawable.h"
#include <vector>

/*
Denna klass �r t�nkt att hantera de olika Object och ljusk�llor i scenen
i form av beh�llare, objekten kallar sin draw() funktion och kan �ndra sitt inneh�ll mm

*/

class GeometryHandler
{

private:
	std::vector<Drawable*> drawableObjects; //holds Geometry objects
	

public:
	GeometryHandler(RenderEngine* engine);
	~GeometryHandler();

};