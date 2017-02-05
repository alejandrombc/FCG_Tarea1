//Alejandro Barone - CI 24206267

#include "Skybox.h"

GLfloat skyboxVertices[] = {
	// Posiciones          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

GLuint Skybox::loadCubemap(vector<const GLchar*> faces)
{
	ilInit();
	ILuint imageID;
	GLuint textureID;
	glGenTextures(1, &textureID);
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	for (GLuint i = 0; i < faces.size(); i++) {
		success = ilLoadImage(faces[i]);
		if (success)
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
			if (!success) {
				error = ilGetError();
				cout << "Image conversion fails" << endl;
			}
			//glActiveTexture(GL_TEXTURE0);

			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT),
				0,
				ilGetInteger(IL_IMAGE_FORMAT),
				GL_UNSIGNED_BYTE,
				ilGetData()
			);
		}
	}

	ilDeleteImages(1, &imageID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;

}

//Iniciar shader de Skybox
void Skybox::loadShaderSkyBox() {
	programSkyBox.loadShader("Shaders/vertSkyshader.ver", CGLSLProgram::VERTEX);
	programSkyBox.loadShader("Shaders/fragSkyshader.frag", CGLSLProgram::FRAGMENT);

	programSkyBox.create_link();

	programSkyBox.enable();

	programSkyBox.addAttribute("position");

	programSkyBox.addUniform("projection");
	programSkyBox.addUniform("view");
	programSkyBox.addUniform("skybox");


	programSkyBox.disable();
}

void Skybox::initSkybox() {
	//Inicializar VAO y VBO del skybox
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	//Guardo texturas en el vector de texturas del cubmap
	faces.push_back("Texturas/left.png");
	faces.push_back("Texturas/right.png");
	faces.push_back("Texturas/top.png");
	faces.push_back("Texturas/bottom.png");
	faces.push_back("Texturas/back.png");
	faces.push_back("Texturas/front.png");
	cubemapTexture = loadCubemap(faces);

	// Creo un nuevo identificador de los shaders del skybox
	programSkyBox.link();
}