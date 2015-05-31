#pragma once
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <stdlib.h>
#include <vector>
#include <GL/glew.h>		// Include GLEW
#include <GLFW/glfw3.h>		// Include GLFW
#include <glm/glm.hpp>		// Include GLM
#include <glm/gtc/matrix_transform.hpp>		// Include GLM
// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3 
#include "shader.hpp"		// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "objects.hpp"		// Wuerfel und Kugel
#include "objloader.hpp"	// 3. wird zum importieren der Teekanne benötigt
#include "texture.hpp"		

using namespace glm;
using namespace std;

class Objekt
{

private:

	GLuint TextureID;
	GLuint programID;
	GLuint Texture;
	GLuint VertexArrayID;
	GLuint vertexbuffer;
	GLuint uvbuffer;

	mat4 MVP;

	vector<glm::vec3> vertices;																	
	vector<glm::vec2> uvs;																				
	vector<glm::vec3> normals;
	
	const char *dateiname;

	double xMinRahmen;
	double yMinRahmen;
	double zMinRahmen;
	double xMaxRahmen;
	double yMaxRahmen;
	double zMaxRahmen;

	double x;
	double y;
	double z;

public:
	mat4 Model;

	

	

	

	double radius;
	
	Objekt(char *texture_name,GLuint programID);
	Objekt(char *objektname, char *textzre_name , GLuint programID);
	//gibt den Speicher wieder frei
	~Objekt(void);
	//legt ein belibiges Objekt an z.B Teekanne
	void Objekt::anlegen(mat4 Projection, mat4 View);
	//Vordefennierte Objekte
	void Objekt::quadrat_anlegen(mat4 Projection, mat4 View);
	//Translation, Skalierung, Roatation
	void Objekt::translate_skalieren(double x = 0.0, double y = 0.0, double z = 0.0, double x_scale = 1.0/1000.0, double y_scale = 1.0/1000.0, double z_scale = 1.0/1000.0);
	void Objekt::objekt_drehen(float angleX, float angleY, float angleZ);

	void Objekt::lampeBewegen(float x, float y, float z, float ydrehen, mat4 lightTrf);

	vector<float>  Objekt::pos();
};

