#include "Interfaz.h"
#include "Modelos.h"

extern Camera camera;
extern vector<Modelos> models;
extern GLint GLFW_WIDTH, GLFW_HEIGHT;

extern glm::mat4 project_mat; //Matriz de Proyeccion
extern glm::mat4 view_mat; //Matriz de View
extern glm::vec3 eye; // Ojo

extern interpolacion currentInterpolacion;
extern especular currentSpecular;
extern difuso currentDifuso;
extern lightT currentLight;

extern int selectedModel; extern int tiempoRotacion; extern int apagarLuz;
extern float scaleT, ejeX, ejeY, ejeZ, ejeXL, ejeYL, ejeZL,
fresnel, roughness, roughnessOren, albido, shininessBP, shininessP,
puntualC, puntualL, puntualQ, innerCut, outerCut;
extern float rotacion[];
extern float color_luz_ambiental[]; extern float color_mat_ambiental[3];
extern float color_luz_difusa[]; extern float color_mat_difusa[3];
extern float color_luz_especular[]; extern float color_mat_especular[3];
extern float rotacion_principal[];
extern float direccionLuz[];
extern float rotacionInicial[];
extern bool selecting, MENU_TRY, boolTexturas;
extern bool keys[1024]; //Todas las teclas

extern GLfloat lastX, lastY;
extern GLfloat deltaTime;
extern bool firstMouse;
extern GLfloat lastFrame;

//<-----------------FUNCIONES DE ANT TWEAK BAR--------------------->

										//Funcion de salir
void TW_CALL exit(void *clientData) {
	exit(1);
}

//Set del autorotar
void TW_CALL SetAutoRotateCB(const void *value, void *clientData) {
	models[selectedModel].autoRotacion = *(const int *)value;
	if (models[selectedModel].autoRotacion) {
		tiempoRotacion = getTimeMs();
		rotacionInicial[0] = rotacion[0];
		rotacionInicial[1] = rotacion[1];
		rotacionInicial[2] = rotacion[2];
		rotacionInicial[3] = rotacion[3];
	}
}

//Funcion de AutoRotate
void TW_CALL GetAutoRotateCB(void *value, void *clientData) {
	(void)clientData; // unused
	*(int *)value = models[selectedModel].autoRotacion;
}

//Funcion para las texturas (aparecer y desaparecer)
void TW_CALL activateTex(void *clientData) {
	boolTexturas = !boolTexturas;
	models[selectedModel].texturizado = boolTexturas;
}

//Funcion para editar especular
void TW_CALL editarE(void *clientData) {
	TwDefine("EditDiffOren visible=false");
	TwDefine("EditSpecBP visible=false");
	TwDefine("EditSpecPhong visible=false");
	TwDefine("EditSpecCook visible=false");
	TwDefine("Figura visible=false");
	if (currentSpecular == 1) TwDefine("EditSpecBP visible=true");
	else if (currentSpecular == 2) TwDefine("EditSpecPhong visible=true");
	else if (currentSpecular == 3) TwDefine("EditSpecCook visible=true");
}

//Funcion para editar difuso
void TW_CALL editarD(void *clientData) {

	TwDefine("EditDiffOren visible=false");
	TwDefine("EditSpecBP visible=false");
	TwDefine("EditSpecPhong visible=false");
	TwDefine("EditSpecCook visible=false");

	if (currentDifuso == 2) {
		TwDefine("Figura visible=false");
		TwDefine("EditDiffOren visible=true");
	}
}

//Funcion para editar especular no principal (NP)
void TW_CALL editarENP(void *clientData) {
	TwDefine("EditDiffOren2 visible=false");
	TwDefine("EditSpecBP2 visible=false");
	TwDefine("EditSpecPhong2 visible=false");
	TwDefine("EditSpecCook2 visible=false");
	TwDefine("Figura_NP visible=false");
	if (currentSpecular == 1) TwDefine("EditSpecBP2 visible=true");
	else if (currentSpecular == 2) TwDefine("EditSpecPhong2 visible=true");
	else if (currentSpecular == 3) TwDefine("EditSpecCook2 visible=true");
}

//Funcion para editar difuso no principal (NP)
void TW_CALL editarDNP(void *clientData) {

	TwDefine("EditDiffOren2 visible=false");
	TwDefine("EditSpecBP2 visible=false");
	TwDefine("EditSpecPhong2 visible=false");
	TwDefine("EditSpecCook2 visible=false");

	if (currentDifuso == 2) {
		TwDefine("Figura_NP visible=false");
		TwDefine("EditDiffOren2 visible=true");
	}
}

//Funcion para editar luz
void TW_CALL editarL(void *clientData) {

	if (currentLight == 2) {
		TwDefine("Sol visible=false");
		TwDefine("EditLPuntual visible=true");
	}
	else if (currentLight == 3) {
		TwDefine("Sol visible=false");
		TwDefine("EditLReflector visible=true");
	}
}

//Funcion para volver menu
void TW_CALL volver(void *clientData) {
	TwDefine("EditDiffOren visible=false");
	TwDefine("EditSpecBP visible=false");
	TwDefine("EditSpecPhong visible=false");
	TwDefine("EditSpecCook visible=false");
	TwDefine("Figura visible=false");
	TwDefine("Figura_NP visible=false");
	TwDefine("Sol visible=false");
	selecting = false;
	selectedModel = 0;

	TwDefine("Menu visible=true");
}

//Funcion para volver a Figura
void TW_CALL volverF(void *clientData) {
	TwDefine("EditDiffOren visible=false");
	TwDefine("EditSpecBP visible=false");
	TwDefine("EditSpecPhong visible=false");
	TwDefine("EditSpecCook visible=false");
	TwDefine("Figura visible=false");
	TwDefine("Figura_NP visible=false");
	TwDefine("Sol visible=false");

	TwDefine("Figura visible=true");
}

//Funcion para volver a Figura_NP
void TW_CALL volverNP(void *clientData) {
	TwDefine("EditDiffOren2 visible=false");
	TwDefine("EditSpecBP2 visible=false");
	TwDefine("EditSpecPhong2 visible=false");
	TwDefine("EditSpecCook2 visible=false");
	TwDefine("Figura visible=false");
	TwDefine("Figura_NP visible=false");
	TwDefine("Sol visible=false");

	TwDefine("Figura_NP visible=true");
}


//Funcion para volver menu SOL
void TW_CALL volverL(void *clientData) {
	TwDefine("EditDiffOren visible=false");
	TwDefine("EditSpecBP visible=false");
	TwDefine("EditSpecPhong visible=false");
	TwDefine("EditSpecCook visible=false");
	TwDefine("Figura visible=false");
	TwDefine("Figura_NP visible=false");
	TwDefine("EditLReflector visible=false");
	TwDefine("EditLPuntual visible=false");



	TwDefine("Sol visible=true");
}

//Sets y gets de auto trasladar
void TW_CALL SetAutoTransCB(const void *value, void *clientData) {
	models[selectedModel].autoTranslate = *(const int *)value;
	if (models[selectedModel].autoTranslate) {
		if (selectedModel == 4) {
			models[selectedModel].ejeX = -30.50;
			models[selectedModel].ejeY = 30.25;
			models[selectedModel].ejeZ = 30.0;
		}
		else if (selectedModel == 3) {
			models[selectedModel].ejeX = 30.50;
			models[selectedModel].ejeY = 24.25;
			models[selectedModel].ejeZ = -18.0;
		}
	}
}

void TW_CALL GetAutoTransCB(void *value, void *clientData) {
	(void)clientData; // unused
	*(int *)value = models[selectedModel].autoTranslate;
}

//Sets y gets de auto escalar
void TW_CALL SetAutoScaleCB(const void *value, void *clientData) {
	models[selectedModel].autoScale = *(const int *)value;
	if (models[selectedModel].autoScale) {
		if (selectedModel == 1) {
			models[selectedModel].scaleT = 10;
		}
		else if (selectedModel == 6) {
			models[selectedModel].scaleT = 10;
		}
	}
}

void TW_CALL GetAutoScaleCB(void *value, void *clientData) {
	(void)clientData; // unused
	*(int *)value = models[selectedModel].autoScale;
}

//Sets y gets de apagar luz
void TW_CALL SetAutoApagar(const void *value, void *clientData) {
	apagarLuz = *(const int *)value;
}

void TW_CALL GetAutoApagar(void *value, void *clientData) {
	(void)clientData; // unused
	*(int *)value = apagarLuz;
}


//Chequear si el click le dio a un objeto
void Interfaz::checkSelectedObject(GLint i) {
	TwDefine("EditDiffOren visible=false");
	TwDefine("EditSpecBP visible=false");
	TwDefine("EditSpecPhong visible=false");
	TwDefine("EditSpecCook visible=false");
	TwDefine("EditLPuntual visible=false");
	TwDefine("EditLReflector visible=false");

	TwDefine("EditDiffOren2 visible=false");
	TwDefine("EditSpecBP2 visible=false");
	TwDefine("EditSpecPhong2 visible=false");
	TwDefine("EditSpecCook2 visible=false");

	if (i != selectedModel && selecting) {
		selecting = true;
		TwDefine("Menu visible=false");
		TwDefine("Figura visible=false");
		TwDefine("Figura_NP visible=false");
		TwDefine("Sol visible=false");
	}
	else selecting = !selecting;
	selectedModel = i;

	if (selecting && i == 5) {
		TwDefine("Menu visible=false");
		TwDefine("Figura visible=true");
		TwDefine("Sol visible=false");


		selecting = true;
		rotacion_principal[0] = models[selectedModel].rotacion[0];
		rotacion_principal[1] = models[selectedModel].rotacion[1];
		rotacion_principal[2] = models[selectedModel].rotacion[2];
		rotacion_principal[3] = models[selectedModel].rotacion[3];
		scaleT = models[selectedModel].scaleT;
		ejeX = models[selectedModel].ejeX;
		ejeY = models[selectedModel].ejeY;
		ejeZ = models[selectedModel].ejeZ;

		boolTexturas = models[selectedModel].texturizado;

		color_mat_ambiental[0] = models[selectedModel].color_ambiental[0];
		color_mat_ambiental[1] = models[selectedModel].color_ambiental[1];
		color_mat_ambiental[2] = models[selectedModel].color_ambiental[2];

		color_mat_difusa[0] = models[selectedModel].color_difuso[0];
		color_mat_difusa[1] = models[selectedModel].color_difuso[1];
		color_mat_difusa[2] = models[selectedModel].color_difuso[2];

		color_mat_especular[0] = models[selectedModel].color_especular[0];
		color_mat_especular[1] = models[selectedModel].color_especular[1];
		color_mat_especular[2] = models[selectedModel].color_especular[2];

		shininessBP = models[selectedModel].shinyBP_M;
		shininessP = models[selectedModel].shinyP_M;
		fresnel = models[selectedModel].fresnel_M;
		albido = models[selectedModel].albido_M;
		roughnessOren = models[selectedModel].roughnessOren_M;
		roughness = models[selectedModel].roughness_M;
		currentDifuso = models[selectedModel].currentDifuso_M;
		currentSpecular = models[selectedModel].currentSpecular_M;
	}
	else if (selecting && i == 7) {
		TwDefine("Menu visible=false");
		TwDefine("Figura visible=false");
		TwDefine("Figura_NP visible=false");
		TwDefine("Sol visible=true");
		selecting = true;

		ejeXL = models[7].ejeX;
		ejeYL = models[7].ejeY;
		ejeZL = models[7].ejeZ;


	}
	else if (selecting && i != 5) {
		TwDefine("Menu visible=false");
		TwDefine("Figura_NP visible=true");
		TwDefine("Sol visible=false");

		selecting = true;
		if (models[selectedModel].autoRotacion) {
			rotacion[0] = models[selectedModel].rotacion[0];
			rotacion[1] = models[selectedModel].rotacion[1];
			rotacion[2] = models[selectedModel].rotacion[2];
			rotacion[3] = models[selectedModel].rotacion[3];
		}
		scaleT = models[selectedModel].scaleT;

		ejeX = models[selectedModel].ejeX;
		ejeY = models[selectedModel].ejeY;
		ejeZ = models[selectedModel].ejeZ;

		boolTexturas = models[selectedModel].texturizado;

		color_mat_ambiental[0] = models[selectedModel].color_ambiental[0];
		color_mat_ambiental[1] = models[selectedModel].color_ambiental[1];
		color_mat_ambiental[2] = models[selectedModel].color_ambiental[2];

		color_mat_difusa[0] = models[selectedModel].color_difuso[0];
		color_mat_difusa[1] = models[selectedModel].color_difuso[1];
		color_mat_difusa[2] = models[selectedModel].color_difuso[2];

		color_mat_especular[0] = models[selectedModel].color_especular[0];
		color_mat_especular[1] = models[selectedModel].color_especular[1];
		color_mat_especular[2] = models[selectedModel].color_especular[2];

		shininessBP = models[selectedModel].shinyBP_M;
		shininessP = models[selectedModel].shinyP_M;
		fresnel = models[selectedModel].fresnel_M;
		albido = models[selectedModel].albido_M;
		roughnessOren = models[selectedModel].roughnessOren_M;
		roughness = models[selectedModel].roughness_M;
		currentDifuso = models[selectedModel].currentDifuso_M;
		currentSpecular = models[selectedModel].currentSpecular_M;

		TwDefine("Figura_NP/autoScale visible=false");
		TwDefine("Figura_NP/autoTranslate  visible=false");
		TwDefine("Figura_NP/autoRotate  visible=false");

		if (selectedModel == 3 || selectedModel == 2 || selectedModel == 1) TwDefine("Figura_NP/autoRotate visible=true");
		if (selectedModel == 4 || selectedModel == 3) TwDefine("Figura_NP/autoTranslate visible=true");
		if (selectedModel == 1 || selectedModel == 6) TwDefine("Figura_NP/autoScale visible=true");

		//Mantener posicion en autorotate
		tiempoRotacion = getTimeMs();
		rotacionInicial[0] = rotacion[0];
		rotacionInicial[1] = rotacion[1];
		rotacionInicial[2] = rotacion[2];
		rotacionInicial[3] = rotacion[3];
	}
	else {
		TwDefine("Menu visible=true");
		TwDefine("Figura visible=false");
		TwDefine("Figura_NP visible=false");
		TwDefine("Sol visible=false");
		if (models[selectedModel].autoRotacion) {
			models[selectedModel].rotacion[0] = rotacion[0];
			models[selectedModel].rotacion[1] = rotacion[1];
			models[selectedModel].rotacion[2] = rotacion[2];
			models[selectedModel].rotacion[3] = rotacion[3];
		}
		if (selectedModel != 7) {
			models[selectedModel].scaleT = scaleT;
			boolTexturas = models[selectedModel].texturizado;

			models[selectedModel].ejeX = ejeX;
			models[selectedModel].ejeY = ejeY;
			models[selectedModel].ejeZ = ejeZ;
		}
		else {
			models[selectedModel].ejeX = ejeXL;
			models[selectedModel].ejeY = ejeYL;
			models[selectedModel].ejeZ = ejeZL;
		}
		selecting = false;
	}
}

//Funcion de reshape
void Interfaz::reshape(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLFW_HEIGHT = height;
	GLFW_WIDTH = width;

	project_mat = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

	glm::mat4 model_mat;
	glm::vec3 norm(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	gluLookAt(eye[0], eye[1], eye[2], norm[0], norm[1], norm[2], up[0], up[1], up[2]);

	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.5, 0.5, 0.5, 0);
	glColor3f(1.0, 1.0, 1.0);
	TwWindowSize(width, height);
}

//Funcion de motionfunc (mueve la camara con el mouse)
void Interfaz::motionFunc(GLFWwindow* window, int button, int action, int mods) {
	if (MENU_TRY) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (TwEventMouseButtonGLFW(button, action)) {
			return;
		}
		else {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				double mouse_x, mouse_y;
				glfwGetCursorPos(window, &mouse_x, &mouse_y);
				
				GLint indice_modelo;
				glReadPixels(mouse_x, GLFW_HEIGHT - mouse_y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &indice_modelo);

				if (indice_modelo != 0) checkSelectedObject(indice_modelo);
			}
		}

	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		if (firstMouse)
		{
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		GLfloat xoffset = x - lastX;
		GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

		lastX = x;
		lastY = y;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

}

//Funcion de MotionPassive del mouse
void Interfaz::passiveMotionFunc(GLFWwindow* window, double x, double y) {
	if (MENU_TRY) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (TwEventMousePosGLFW(x, y)) return;
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
		}

		if (x < 100) {
			glfwSetCursorPos(window, GLFW_WIDTH - 100, y);
			lastX = GLFW_WIDTH - 100;
		}
		else if (x > GLFW_WIDTH - 100) {
			glfwSetCursorPos(window, 100, y);
			lastX = 100;
		}

		if (y < 100) {
			glfwSetCursorPos(window, x, GLFW_HEIGHT - 100);
			lastY = GLFW_HEIGHT - 100;
		}
		else if (y > GLFW_HEIGHT - 100) {
			glfwSetCursorPos(window, x, 100);
			lastY = 100;
		}

		GLfloat xoffset = x - lastX;
		GLfloat yoffset = lastY - y;  // Reversed since y-coordinates go from bottom to left

		yoffset = (yoffset < 100 && yoffset > -100) ? yoffset : 0;
		xoffset = (xoffset < 100 && xoffset > -100) ? xoffset : 0;

		lastX = x;
		lastY = y;
		camera.ProcessMouseMovement(xoffset, yoffset, true);
	}
}

//Teclado (para agarrar notas, y escape para cerrar el juego)
void Interfaz::keyFunc(GLFWwindow* window, int Key, int iScancode, int iAction, int iMods) {
	if ((iAction == GLFW_PRESS && Key == GLFW_KEY_M)) {
		MENU_TRY = !MENU_TRY;
		keys['W'] = false; keys['w'] = false;
		keys['S'] = false; keys['a'] = false;
		keys['A'] = false; keys['s'] = false;
		keys['D'] = false; keys['d'] = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (TwEventKeyGLFW(Key, iAction)) return;
	}
	else if (!MENU_TRY) {
		MENU_TRY = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (iAction == GLFW_PRESS) keys[Key] = true;
		if (iAction == GLFW_RELEASE) keys[Key] = false;
	}
}

//Para poner numeros en AntTweakBar
void Interfaz::obtenerTecla(GLFWwindow* window, int codepoint) {
	TwEventCharGLFW(codepoint, GLFW_PRESS);
}

//Iniciar el menu de Anttweakbar
void Interfaz::initAntTweakParam() {
	//Ant Tweak Bar
	TwBar *menuTW, *modelTW;

	TwInit(TW_OPENGL, NULL);

	menuTW = TwNewBar("Menu");
	TwWindowSize(800, 600);
	//Botones del menu inicial en AntTweakBar
	TwDefine(" GLOBAL help='Tarea 1 FCG - Alejandro Barone' "); // Mensaje de ventana
	TwDefine(" Menu size='200 150' color='0 0 0' "); // Color y tamano de tweak bar
													 //Tipo de interpolacion (Gouraud, Phong)
	{
		TwEnumVal styleShader[3] = { { Gouraud, "Gouraud" },{ Phong, "Phong" } };
		TwType interpolacion = TwDefineEnum("InterpolacionType", styleShader, 2);
		TwAddVarRW(menuTW, "Interpolacion", interpolacion, &currentInterpolacion, "");
	}

	TwAddButton(menuTW, "exit", exit, NULL, " label='Salir' key=Esc");

	//Las ediciones de los difusos
	modelTW = TwNewBar("EditDiffOren");
	TwDefine("EditDiffOren visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Oren-Nayar'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "albido", TW_TYPE_FLOAT, &albido, "min=0.01 step=0.01 max=1.0 label='Albido'");
	TwAddVarRW(modelTW, "rugosidad", TW_TYPE_FLOAT, &roughnessOren, "min=0.01 step=0.01 max=1.0 label='Rugosidad'");
	TwAddButton(modelTW, "Volver", volverF, NULL, " label='Volver' key=Esc");

	//Las ediciones de los especulares
	modelTW = TwNewBar("EditSpecCook");
	TwDefine("EditSpecCook visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Cook-Torrance'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "fresnel", TW_TYPE_FLOAT, &fresnel, "min=0.01 step=0.01 max=1.0 label='Fresnel'");
	TwAddVarRW(modelTW, "rugo", TW_TYPE_FLOAT, &roughness, "min=0.01 step=0.01 max=1.0 label='Rugosidad'");
	TwAddButton(modelTW, "Volver", volverF, NULL, " label='Volver' key=Esc");

	modelTW = TwNewBar("EditSpecPhong");
	TwDefine("EditSpecPhong visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Phong'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "brilloP", TW_TYPE_FLOAT, &shininessP, "min=0 step=1.0 max=128.0 label='Brillo'");
	TwAddButton(modelTW, "Volver", volverF, NULL, " label='Volver' key=Esc");


	modelTW = TwNewBar("EditSpecBP");
	TwDefine("EditSpecBP visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Blinn-Phong'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "brilloBP", TW_TYPE_FLOAT, &shininessBP, "min=0 step=1.0 max=128.0 label='Brillo'");
	TwAddButton(modelTW, "Volver", volverF, NULL, " label='Volver' key=Esc");

	//Figura
	modelTW = TwNewBar("Figura");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwDefine("Figura visible=false size='270 500' position='20 20' color='128 0 0' label='Modelo principal'");

	TwAddVarRW(modelTW, "scale", TW_TYPE_FLOAT, &scaleT, "min=0.01 step=0.01 label='Escalar'");
	TwAddVarRW(modelTW, "ejeX", TW_TYPE_FLOAT, &ejeX, "step=0.01 label='x' group='Trasladar'");
	TwAddVarRW(modelTW, "ejeY", TW_TYPE_FLOAT, &ejeY, "step=0.01 label='y' group='Trasladar'");
	TwAddVarRW(modelTW, "ejeZ", TW_TYPE_FLOAT, &ejeZ, "step=0.01 label='z' group='Trasladar'");
	TwAddVarRW(modelTW, "rotation", TW_TYPE_QUAT4F, &rotacion_principal, " label='Rotación' opened=true group='Rotar'");
	TwAddButton(modelTW, "activateTex", activateTex, NULL, " label='Activar' group='Texturas'");
	TwAddVarRW(modelTW, "texturizado", TW_TYPE_BOOL8, &boolTexturas, "label='Texturas' readonly=true group='Texturas'");
	TwAddVarRW(modelTW, "Color Material Ambiental", TW_TYPE_COLOR3F, &color_mat_ambiental, "label='Ambiental' group='Material'");
	TwAddVarRW(modelTW, "Color Material Difuso", TW_TYPE_COLOR3F, &color_mat_difusa, "label='Difuso' group='Material'");
	TwAddVarRW(modelTW, "Color Material Especular", TW_TYPE_COLOR3F, &color_mat_especular, "label='Especular' group='Material'");


	//Tipo de especular (Phong, Blinn-Phong, Cook-Torrance)
	{
		TwEnumVal styleSpecular[3] = { { BlinnPhong, "Blinn-Phong" },{ PhongS, "Phong" },{ CookTorrance, "Cook-Torrance" } };
		TwType specular = TwDefineEnum("SpecularType", styleSpecular, 3);
		TwAddVarRW(modelTW, "Modelo especular", specular, &currentSpecular, "group='Especular'");
		TwAddButton(modelTW, "SpecularR", editarE, NULL, " label='Editar' key=Esc group='Especular'");
	}

	//Tipo de difuso (Lambert, Oren-Nayar)
	{
		TwEnumVal styleDifuso[3] = { { Lambert, "Lambert" },{ OrenNayar, "Oren-Nayar" } };
		TwType difuso = TwDefineEnum("DifusoType", styleDifuso, 2);
		TwAddVarRW(modelTW, "Modelo difuso", difuso, &currentDifuso, "group='Difuso'");
		TwAddButton(modelTW, "DifusoR", editarD, NULL, " label='Editar' key=Esc group='Difuso'");

	}

	TwAddButton(modelTW, "Volver", volver, NULL, " label='Volver' key=Esc");


	//Menu de Luz
	modelTW = TwNewBar("Sol");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwDefine("Sol visible=false size='270 300' position='20 20' color='128 0 0' label='Iluminacion'");
	TwAddVarRW(modelTW, "ejeXL", TW_TYPE_FLOAT, &ejeXL, "step=0.01 label='x' group='Trasladar luz' group='Luz'");
	TwAddVarRW(modelTW, "ejeYL", TW_TYPE_FLOAT, &ejeYL, "step=0.01 label='y' group='Trasladar luz' group='Luz'");
	TwAddVarRW(modelTW, "ejeZL", TW_TYPE_FLOAT, &ejeZL, "step=0.01 label='z' group='Trasladar luz' group='Luz'");

	//Tipo de luz (Direccional, Puntual, Reflector)
	{
		TwEnumVal styleLight[3] = { { Direccional, "Direccional" },{ Puntual, "Puntual" },{ Reflector, "Reflector" } };
		TwType lighttype = TwDefineEnum("LightType", styleLight, 3);
		TwAddVarRW(modelTW, "Tipo de luz", lighttype, &currentLight, "group='Luz'");
		TwAddButton(modelTW, "EditarR", editarL, NULL, " label='Editar' key=Esc group='Luz'");
	}
	TwAddVarCB(modelTW, "autoTranslate", TW_TYPE_BOOL32, SetAutoApagar, GetAutoApagar, NULL, "label='Prender/Apagar luz'");
	//Color del material, ambiental, difuso y especular
	TwAddVarRW(modelTW, "Color Luz Ambiental", TW_TYPE_COLOR3F, &color_luz_ambiental, "label='Ambiental'");
	TwAddVarRW(modelTW, "Color Luz Difuso", TW_TYPE_COLOR3F, &color_luz_difusa, "label='Difuso'");
	TwAddVarRW(modelTW, "Color Luz Especular", TW_TYPE_COLOR3F, &color_luz_especular, "label='Especular'");
	TwAddButton(modelTW, "Volver", volver, NULL, " label='Volver' key=Esc");

	//Editar Luz Puntual
	modelTW = TwNewBar("EditLPuntual");
	TwDefine("EditLPuntual visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Luz Puntual'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "puntualC", TW_TYPE_FLOAT, &puntualC, "min=0 step=0.01 max=1.0 label='Constante'");
	TwAddVarRW(modelTW, "puntualL", TW_TYPE_FLOAT, &puntualL, "min=0 step=0.001 max=1.0 label='Lineal'");
	TwAddVarRW(modelTW, "puntualQ", TW_TYPE_FLOAT, &puntualQ, "min=0 step=0.0001 max=1.0 label='Cuadratica'");
	TwAddButton(modelTW, "Volver", volverL, NULL, " label='Volver' key=Esc");

	//Editar Luz Reflector
	modelTW = TwNewBar("EditLReflector");
	TwDefine("EditLReflector visible=false size='250 300' position='20 20' color='128 0 0' label='Editar parametros Luz Reflector'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "puntualC", TW_TYPE_FLOAT, &puntualC, "min=0 step=0.01 max=1.0 label='Constante'");
	TwAddVarRW(modelTW, "puntualL", TW_TYPE_FLOAT, &puntualL, "min=0 step=0.001 max=1.0 label='Lineal'");
	TwAddVarRW(modelTW, "puntualQ", TW_TYPE_FLOAT, &puntualQ, "min=0 step=0.0001 max=1.0 label='Cuadratica'");
	TwAddVarRW(modelTW, "innerCut", TW_TYPE_FLOAT, &innerCut, "min=0 step=0.001 max=1.0 label='In coseno'");
	TwAddVarRW(modelTW, "outerCut", TW_TYPE_FLOAT, &outerCut, "min=0 step=0.001 max=1.0 label='Out coseno'");
	TwAddVarRW(modelTW, "LightDir", TW_TYPE_DIR3F, &direccionLuz,
		" label='Direccion spotlight' opened=true help='Cambiar direccion luz' ");
	TwAddButton(modelTW, "Volver", volverL, NULL, " label='Volver' key=Esc");


	//Figuras no principales
	modelTW = TwNewBar("Figura_NP");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwDefine("Figura_NP visible=false size='270 350' position='20 20' color='128 0 0' label='Modelos no principales'");
	TwAddVarCB(modelTW, "autoRotate", TW_TYPE_BOOL32, SetAutoRotateCB, GetAutoRotateCB, NULL, "visible=false label='Auto-rotacion'");
	TwAddVarCB(modelTW, "autoTranslate", TW_TYPE_BOOL32, SetAutoTransCB, GetAutoTransCB, NULL, "visible=false label='Auto-traslacion'");
	TwAddVarCB(modelTW, "autoScale", TW_TYPE_BOOL32, SetAutoScaleCB, GetAutoScaleCB, NULL, "visible=false label='Auto-escalamiento'");


	TwAddButton(modelTW, "activateTex", activateTex, NULL, " label='Activar' group='Texturas'");
	TwAddVarRW(modelTW, "Color Material Ambiental", TW_TYPE_COLOR3F, &color_mat_ambiental, "label='Ambiental' group='Material'");
	TwAddVarRW(modelTW, "Color Material Difuso", TW_TYPE_COLOR3F, &color_mat_difusa, "label='Difuso' group='Material'");
	TwAddVarRW(modelTW, "Color Material Especular", TW_TYPE_COLOR3F, &color_mat_especular, "label='Especular' group='Material'");



	//Tipo de especular (Phong, Blinn-Phong, Cook-Torrance)
	{
		TwEnumVal styleSpecularNP[3] = { { BlinnPhong, "Blinn-Phong" },{ PhongS, "Phong" },{ CookTorrance, "Cook-Torrance" } };
		TwType specularNP = TwDefineEnum("SpecularType", styleSpecularNP, 3);
		TwAddVarRW(modelTW, "Modelo especular", specularNP, &currentSpecular, "group='Especular'");
		TwAddButton(modelTW, "SpecularR", editarENP, NULL, " label='Editar' key=Esc group='Especular'");
	}

	//Tipo de difuso (Lambert, Oren-Nayar)
	{
		TwEnumVal styleDifusoNP[3] = { { Lambert, "Lambert" },{ OrenNayar, "Oren-Nayar" } };
		TwType difusoNP = TwDefineEnum("DifusoType", styleDifusoNP, 2);
		TwAddVarRW(modelTW, "Modelo difuso", difusoNP, &currentDifuso, "group='Difuso'");
		TwAddButton(modelTW, "DifusoR", editarDNP, NULL, " label='Editar' key=Esc group='Difuso'");

	}

	TwAddVarRW(modelTW, "texturizado", TW_TYPE_BOOL8, &boolTexturas, "label='Texturas' readonly=true group='Texturas'");


	//Boton de salir
	TwAddButton(modelTW, "Volver", volver, NULL, " label='Volver' key=Esc");


	//Ediciones de figuras no principales
	//Las ediciones de los difusos
	modelTW = TwNewBar("EditDiffOren2");
	TwDefine("EditDiffOren2 visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Oren-Nayar'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "albido", TW_TYPE_FLOAT, &albido, "min=0.01 step=0.01 max=1.0 label='Albido'");
	TwAddVarRW(modelTW, "rugosidad", TW_TYPE_FLOAT, &roughnessOren, "min=0.01 step=0.01 max=1.0 label='Rugosidad'");
	TwAddButton(modelTW, "Volver", volverNP, NULL, " label='Volver' key=Esc");

	//Las ediciones de los especulares
	modelTW = TwNewBar("EditSpecCook2");
	TwDefine("EditSpecCook2 visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Cook-Torrance'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "fresnel", TW_TYPE_FLOAT, &fresnel, "min=0.01 step=0.01 max=1.0 label='Fresnel'");
	TwAddVarRW(modelTW, "rugo", TW_TYPE_FLOAT, &roughness, "min=0.01 step=0.01 max=1.0 label='Rugosidad'");
	TwAddButton(modelTW, "Volver", volverNP, NULL, " label='Volver' key=Esc");

	modelTW = TwNewBar("EditSpecPhong2");
	TwDefine("EditSpecPhong2 visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Phong'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "brilloP", TW_TYPE_FLOAT, &shininessP, "min=0 step=1.0 max=128.0 label='Brillo'");
	TwAddButton(modelTW, "Volver", volverNP, NULL, " label='Volver' key=Esc");


	modelTW = TwNewBar("EditSpecBP2");
	TwDefine("EditSpecBP2 visible=false size='200 150' position='20 20' color='128 0 0' label='Editar parametros Blinn-Phong'");
	TwWindowSize(GLFW_WIDTH, GLFW_HEIGHT);
	TwAddVarRW(modelTW, "brilloBP", TW_TYPE_FLOAT, &shininessBP, "min=0 step=1.0 max=128.0 label='Brillo'");
	TwAddButton(modelTW, "Volver", volverNP, NULL, " label='Volver' key=Esc");


}



