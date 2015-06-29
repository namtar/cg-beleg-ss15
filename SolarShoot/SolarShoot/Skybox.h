#pragma once

// #include "skyboxTexture.h"
#include "commonHeader.h"
#include "skyboxVertexBufferObject.h"
#include "texture.hpp"

/********************************

Class:	CSkybox

Purpose: Class for using skybox.
http://www.mbsoftworks.sk/index.php?page=tutorials&series=1&tutorial=13

********************************/

class CSkybox
{
public:
	void loadSkybox(string a_sDirectory, string a_sFront, string a_sBack, string a_sLeft, string a_sRight, string a_sTop, string a_sBottom);
	void renderSkybox();

	void releaseSkybox();
private:
	UINT uiVAO;
	CVertexBufferObject vboRenderData;
	// CTexture tTextures[6];
	// string sDirectory;
	// string sFront, sBack, sLeft, sRight, sTop, sBottom;

	GLuint front;
	GLuint back;
	GLuint right;
	GLuint left;
	GLuint top;
	GLuint bottom;

};