#pragma once
#include "Main.h"
class Skybox {
public:
	CGLSLProgram programSkyBox;
	GLuint skyboxVAO, skyboxVBO;
	vector<const GLchar*> faces;
	GLuint cubemapTexture;
	GLuint loadCubemap(vector<const GLchar*> faces);
	void loadShaderSkyBox();
	void initSkybox();
};

