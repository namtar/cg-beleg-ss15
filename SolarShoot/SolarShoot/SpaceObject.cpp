#include "SpaceObject.hpp"


SpaceObject::SpaceObject(GLuint programId)
{
	this->programId = programId;
}


SpaceObject::~SpaceObject(void)
{
}

void SpaceObject::draw(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix) 
{

	MVP = ProjectionMatrix * ViewMatrix * Model; 

	glUniformMatrix4fv(glGetUniformLocation(programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "V"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "M"), 1, GL_FALSE, &Model[0][0]);

	drawCube();
}

void SpaceObject::translate(GLfloat x, GLfloat y, GLfloat z)
{
	// glTranslatef(x, y, z);

	Model = glm::translate(Model, glm::vec3(x, y, z));
}

void SpaceObject::scale()
{

}

void SpaceObject::rotate()
{
}