#include "SkyboxSich.h"


Skybox::Skybox(void)
{
//	GLuint rightTexture = loadBMP_custom("material/sb/right.jpg");


	vector<const GLchar*> faces;
	faces.push_back("material/sb/right.jpg");
	faces.push_back("material/sb/left.jpg");
	faces.push_back("material/sb/top.jpg");
	faces.push_back("material/sb/bottom.jpg");
	faces.push_back("material/sb/back.jpg");
	faces.push_back("material/sb/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);
}


Skybox::~Skybox(void)
{
}

GLuint Skybox::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//	for (GLuint i = 0; i < faces.size(); i++)
//	{
//		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
//		glTexImage2D(
//			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
//			                              GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
//		);
//	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

//void Skybox::loadSkybox(string a_sDirectory, string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom)
//{
//	tTextures[0].loadTexture2D(a_sDirectory+a_sFront);
//	tTextures[1].loadTexture2D(a_sDirectory+a_sBack);
//	tTextures[2].loadTexture2D(a_sDirectory+a_sLeft);
//	tTextures[3].loadTexture2D(a_sDirectory+a_sRight);
//	tTextures[4].loadTexture2D(a_sDirectory+a_sTop);
//	tTextures[5].loadTexture2D(a_sDirectory+a_sBottom);
//
//	sDirectory = a_sDirectory;
//
//	sFront = a_sFront;
//	sBack = a_sBack;
//	sLeft = a_sLeft;
//	sRight = a_sRight;
//	sTop = a_sTop;
//	sBottom = a_sBottom;

//	FOR(i, 6)
//	{
//		tTextures[i].setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR);
//		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		tTextures[i].setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	}
//
//	glGenVertexArrays(1, &uiVAO);
//	glBindVertexArray(uiVAO);
//
//	vboRenderData.createVBO();
//	vboRenderData.bindVBO();
//
//	glm::vec3 vSkyBoxVertices[24] = 
//	{
//		// Front face
//		glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
//		// Back face
//		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f),
//		// Left face
//		glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f),
//		// Right face
//		glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(50.0f, -50.0f, 50.0f),
//		// Top face
//		glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(50.0f, 50.0f, 50.0f),
//		// Bottom face
//		glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(-50.0f, -50.0f, 50.0f),
//	};
//	glm::vec2 vSkyBoxTexCoords[4] =
//	{
//		glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 0.0f)
//	};
//
//	glm::vec3 vSkyBoxNormals[6] = 
//	{
//		glm::vec3(0.0f, 0.0f, -1.0f),
//		glm::vec3(0.0f, 0.0f, 1.0f),
//		glm::vec3(1.0f, 0.0f, 0.0f),
//		glm::vec3(-1.0f, 0.0f, 0.0f),
//		glm::vec3(0.0f, -1.0f, 0.0f),
//		glm::vec3(0.0f, 1.0f, 0.0f)
//	};
//
//	FOR(i, 24)
//	{
//		vboRenderData.addData(&vSkyBoxVertices[i], sizeof(glm::vec3));
//		vboRenderData.addData(&vSkyBoxTexCoords[i%4], sizeof(glm::vec2));
//		vboRenderData.addData(&vSkyBoxNormals[i/4], sizeof(glm::vec3));
//	}
//
//	vboRenderData.uploadDataToGPU(GL_STATIC_DRAW);
//
//	// Vertex positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), 0);
//	// Texture coordinates
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)sizeof(glm::vec3));
//	// Normal vectors
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3)+sizeof(glm::vec2), (void*)(sizeof(glm::vec3)+sizeof(glm::vec2)));
//}

void Skybox::renderSkybox()
{
	glDepthMask(0);
	//   glBindVertexArray(uiVAO); 
	FOR(i, 6)
	{
		//      tTextures[i].bindTexture(); 
		glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
	}
	glDepthMask(1);
}

void Skybox::releaseSkybox()
{
}
