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

	GLuint programId;
	glm::vec3 ObjectColor;
	char* textureFileName;
	char* objectFileName;

	void SpaceObject::setupBuffers();

public:

	glm::mat4 Model;

	SpaceObject(GLuint programId, char* textureFileName, char* objectFileName);
	~SpaceObject(void);

	/**
	 * Draws the object.
	 */
	void SpaceObject::draw(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix);
	void SpaceObject::translate(GLfloat x, GLfloat y, GLfloat z);
	void SpaceObject::scale();
	void SpaceObject::rotate();
};

