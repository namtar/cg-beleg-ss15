#pragma once

#include <stdlib.h>
#include <string>
#include <cstring>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "texture.hpp"

using namespace std;

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

class Skybox
{
	// http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=13
	// http://learnopengl.com/#!Advanced-OpenGL/Cubemaps
	// http://learnopengl.com/code_viewer.php?code=advanced/cubemaps_reflection
private:
//	UINT uiVAO;
//	CVertexBufferObject vboRenderData;
//	CTexture tTextures[6];
	string sDirectory;
	string sFront, sBack, sLeft, sRight, sTop, sBottom;

public:
	Skybox(void);
	~Skybox(void);

//	void loadSkybox(string a_sDirectory, string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	GLuint Skybox::loadCubemap(vector<const GLchar*> faces);
	void renderSkybox();

	void releaseSkybox();
};
