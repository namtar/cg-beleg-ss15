#include "SpaceObject.hpp"


SpaceObject::SpaceObject(GLuint programId, char* textureFileName, char* objectFileName)
{
	this->programId = programId;
	this->ObjectColor = glm::vec3(0.0f, 0.0f, 0.0f); // default black
	this->textureFileName = textureFileName;
	this->objectFileName = objectFileName;
	this->rotationValues = glm::vec3(0.0f, 0.0f, 0.0f);
	this->mayDraw = true;
	setupBuffers();
}


SpaceObject::~SpaceObject(void)
{
	// TODO: cleanups
	// glDeleteBuffers
}

void SpaceObject::setupBuffers() 
{		
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	bool res = loadOBJ(this->objectFileName, vertices, uvs, normals);

	// Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer); // Kennung erhalten
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
	// Buffer zugreifbar für die Shader machen
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
	glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0 
	glVertexAttribPointer(0,  // location = 0 
		3,  // Datenformat vec3: 3 floats fuer xyz 
		GL_FLOAT, 
		GL_FALSE, // Fixedpoint data normalisieren ?
		0, // Eckpunkte direkt hintereinander gespeichert
		(void*) 0); // abweichender Datenanfang ?

	GLuint normalbuffer; // Hier alles analog für Normalen in location == 2
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // siehe layout im vertex shader 
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint uvbuffer; // Hier alles analog für Texturkoordinaten in location == 1 (2 floats u und v!)
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // siehe layout im vertex shader 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Load the texture
	Texture = loadBMP_custom(this->textureFileName);
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);	

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	// glUniform1i(glGetUniformLocation(programId, "myTextureSampler"), 0);
}

void SpaceObject::draw(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::mat4 FakeModelMatrix, bool useFakeModelMatrix) 
{	
	if(useFakeModelMatrix)
	{
		LocalModel = FakeModelMatrix * Model;
	} else {
		LocalModel = Model;
	}
	MVP = ProjectionMatrix * ViewMatrix * LocalModel; 

	glUniformMatrix4fv(glGetUniformLocation(programId, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "V"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programId, "M"), 1, GL_FALSE, &LocalModel[0][0]);
	glUniform3f(glGetUniformLocation(programId, "ObjectColor"), ObjectColor[0], ObjectColor[1], ObjectColor[2]);	

	glBindTexture(GL_TEXTURE_2D, Texture);
	glBindVertexArray(vertexArrayId);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // draws the teapot
}

void SpaceObject::translate(GLfloat x, GLfloat y, GLfloat z)
{	
	Model = glm::translate(Model, glm::vec3(x, y, z));
}

void SpaceObject::scale(GLdouble x, GLdouble y, GLdouble z)
{
	Model = glm::scale(Model, glm::vec3(x, y, z));
}

void SpaceObject::rotate(GLfloat angle, GLdouble x, GLdouble y, GLdouble z)
{
	Model = glm::rotate(Model, angle, glm::vec3(x, y, z));
}

void SpaceObject::setRotations(float rotateX, float rotateY, float rotateZ)
{
	this->rotationValues[0] = rotateX;
	this->rotationValues[1] = rotateY;
	this->rotationValues[2] = rotateZ;
}

glm::vec3 SpaceObject::getRotations()
{
	return this->rotationValues;
}

bool SpaceObject::isCollission(float objX, float objY, float objZ) 
{
	// rangeoffset;
	float rangeOffset = 5.0f;

	float modelX = this->LocalModel[3].x;
	float modelY = this->LocalModel[3].y;
	float modelZ = this->LocalModel[3].z;

	if(objX < (modelX + rangeOffset) && objX > (modelX - rangeOffset))
	{
		if(objY < (modelY + rangeOffset) && objY > (modelY - rangeOffset))
		{
			if(objZ < (modelZ + rangeOffset) && objZ > (modelZ - rangeOffset))
			{
				return true;
			}
		}
	}
	return false;
}

void SpaceObject::setMayDraw(bool mayDraw) 
{
	this->mayDraw = mayDraw;
}

bool SpaceObject::isDraw() 
{
	return this->mayDraw;
}
