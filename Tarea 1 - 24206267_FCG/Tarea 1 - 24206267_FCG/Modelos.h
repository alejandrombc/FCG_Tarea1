//Alejandro Barone - CI 24206267
#pragma once
#include "Main.h"

class Modelos {
public:
	//Atributos
	GLuint vbo;
	GLuint vao;
	GLuint vindex;
	bool autoRotacion = false;
	bool autoTranslate = false;
	bool autoScale = false;
	vector<float> vertices;
	vector<float> coord_texturas;
	vector<float> normales_vertice_fin;
	float rotacion[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float color_ambiental[3] = { 0.0, 0.0, 0.0 };
	float color_difuso[3] = { 1.0, 1.0, 1.0 };
	float color_especular[3] = { 1.0, 1.0, 1.0 };
	double scaleT = 10.00;
	float mat[4 * 4];
	float ejeX = 0.0;
	float ejeY = 1.0;
	float ejeZ = 0.0;
	double minX = INT_MAX;
	double maxX = INT_MIN;
	double minY = INT_MAX;
	double maxY = INT_MIN;
	double minZ = INT_MAX;
	double maxZ = INT_MIN;
	bool texturizado = true;
	//Modelos de iluminacion
	float shinyP_M = 32.0;
	float shinyBP_M = 32.0;
	float fresnel_M = 0.8;
	float albido_M = 0.7;
	float roughnessOren_M = 0.2;
	float roughness_M = 0.3;
	especular currentSpecular_M = BlinnPhong;
	difuso currentDifuso_M = Lambert;
	CGLSLProgram programPrincipal; //Shader
	//Todos los ID de textura
	GLuint texture1; GLuint texture2; GLuint texture3; GLuint texture4; GLuint texture5;
	GLuint texture6; GLuint texture7;
	//Metodos
	Modelos();
	~Modelos();
	void initVBO();
	unsigned int LoadTexture(const char* filename, bool text);
	void loadShaderPrincipal();
};


struct vertice {
	double x;
	double y;
	double z;
};

struct face {
	int n_vertex;
	vector<int> f;
	vector<int> t;
};

glm::mat4 scale_en_matriz(float scale_tx);

glm::mat4 translate_en_matriz(float translate_tx, float translate_ty, float translate_tz);

glm::mat4 rotacion_en_matriz(float rotacion_tx, float rotacion_ty, float rotacion_tz, float rotacion_ta);

void setQuat(const float *eje, float angulo, float *quat);

void multiplicarQuat(const float *q1, const float *q2, float *qout);

int getTimeMs();