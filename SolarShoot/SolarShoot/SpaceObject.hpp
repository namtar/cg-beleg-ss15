#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "shader.hpp"
#include "objects.hpp"
#include "texture.hpp"
#include "objloader.hpp"

/*
 * Class for one spaceobject like asteroid oder enemy spaceship.
 */
class SpaceObject
{
private:
	
	glm::mat4 MVP;
	GLuint Texture;
	GLuint programId;
	glm::vec3 ObjectColor;
	glm::vec3 rotationValues;
	bool mayDraw;
	
	// object and texture stuff
	char* textureFileName;
	char* objectFileName;
	GLuint vertexArrayId;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; 	

	void SpaceObject::setupBuffers();

public:

	glm::mat4 Model;

	SpaceObject(GLuint programId, char* textureFileName, char* objectFileName);
	~SpaceObject(void);

	/**
	 * Draws the object.
	 */
	void SpaceObject::draw(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::mat4 FakeModelMatrix, bool useFakeModelMatrix);
	void SpaceObject::translate(GLfloat x, GLfloat y, GLfloat z);
	void SpaceObject::scale(GLdouble x, GLdouble y, GLdouble z);
	void SpaceObject::rotate(GLfloat angle, GLdouble x, GLdouble y, GLdouble z);	
	void SpaceObject::setRotations(float rotateX, float rotateY, float rotateZ);
	glm::vec3 SpaceObject::getRotations();
	bool SpaceObject::isCollission(float objX, float objY, float objZ);
	void SpaceObject::setMayDraw(bool draw);
	bool SpaceObject::isDraw();
};

