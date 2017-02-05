#include "Main.h"
#include "Modelos.h"
#include "Skybox.h"
#include "OBJ.h"
#include "Interfaz.h"

using namespace std;
Modelos mod; Skybox sky; Interfaz inter;
GLint GLFW_WIDTH = 800, GLFW_HEIGHT = 600;


interpolacion currentInterpolacion = Phong;
especular currentSpecular = BlinnPhong;
difuso currentDifuso = Lambert;
lightT currentLight = Direccional;

int selectedModel = 0, tiempoRotacion, apagarLuz = 1;
float scaleT = 1.00, ejeX = 0.0, ejeY = 0.0, ejeZ = 0.0, ejeXL = 0.0, ejeYL = 0.0, ejeZL = 0.0,
fresnel = 0.8, roughness = 0.3, roughnessOren = 0.3, albido = 0.7, shininessBP = 32.0, shininessP = 32.0,
puntualC = 1.0, puntualL = 0.007, puntualQ = 0.0002, innerCut = 0.91, outerCut = 0.82;
float rotacion[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float color_luz_ambiental[] = { 0.0f, 0.0f, 0.0f }; float color_mat_ambiental[3] = { 0.0f, 0.0f, 0.0f };
float color_luz_difusa[] = { 1.0f, 1.0f, 1.0f }; float color_mat_difusa[3] = { 1.0f, 1.0f, 1.0f };
float color_luz_especular[] = { 1.0f, 1.0f, 1.0f }; float color_mat_especular[3] = { 1.0f, 1.0f, 1.0f };
float rotacion_principal[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float direccionLuz[] = { 0.92, 0.39, -0.10 };
float rotacionInicial[] = { 0.0f, 0.0f, 0.0f, 1.0f };
bool selecting = false, MENU_TRY = false, boolTexturas = true;
bool keys[1024]; //Todas las teclas
int tiempo_traslacion[2] = { 0,0 }; int tiempo_escalar[2] = {0,0};

GLfloat lastX = 400, lastY = 300;
GLfloat deltaTime = 0.0f;
bool firstMouse = true;
GLfloat lastFrame = 0.0f;


//<-----------------VARIABLES Y ESTRUCTURAS--------------------->
// Camera
Camera camera(glm::vec3(18.0f, 0.5f, 0.8f));
glm::mat4 project_mat; //Matriz de Proyeccion
glm::mat4 view_mat; //Matriz de View
glm::vec3 eye(0.0f, 0.0f, 2.0f); // Ojo

vector<Modelos> models; //Todos los modelos iran en este ve

//Impresion de pantalla
void initGlew() {
	glEnable(GL_DEPTH_TEST);
	printf("OpenGL Version: %s \n", glGetString(GL_VERSION));
	printf("Vendor: %s \n", glGetString(GL_VENDOR));
	printf("Renderer: %s \n", glGetString(GL_RENDERER));
	printf("GLSL Version: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

//Funcion que dibuja el BB del modelo seleccionado
void draw_bb(GLuint bool_loc) {
	mod.programPrincipal.enable();
	bool_loc = mod.programPrincipal.getLocation("bb");
	glUniform1i(bool_loc, 1);

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].maxZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].maxY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].maxX, models[selectedModel].minY, models[selectedModel].minZ);
	glVertex3f(models[selectedModel].minX, models[selectedModel].minY, models[selectedModel].minZ);
	glEnd();
	mod.programPrincipal.disable();
}

//Funcion que dibuja el modelo VBO con texturas
void draw() {
	if (innerCut < outerCut) innerCut = outerCut;
	for (int i = 0; i < models.size(); i++) { //Para los modelos
		glStencilFunc(GL_ALWAYS, i, -1);
		mod.programPrincipal.enable();

		GLuint view_matr_loc = mod.programPrincipal.getLocation("view_matrix");
		GLuint model_matr_loc = mod.programPrincipal.getLocation("model_matrix");
		GLuint proj_matr_loc = mod.programPrincipal.getLocation("projection_matrix");
		GLuint view_loc = mod.programPrincipal.getLocation("view");

		GLuint bool_loc = mod.programPrincipal.getLocation("bb");
		glUniform1i(bool_loc, 0);
		glUniform1i(mod.programPrincipal.getLocation("currentInterpolacion"), currentInterpolacion);
		glUniform1f(mod.programPrincipal.getLocation("currentLight"), currentLight);

		glUniform1f(mod.programPrincipal.getLocation("pointConstant"), puntualC);
		glUniform1f(mod.programPrincipal.getLocation("pointLinear"), puntualL);
		glUniform1f(mod.programPrincipal.getLocation("pointQuadratic"), puntualQ);

		glUniform1f(mod.programPrincipal.getLocation("innerCut"), innerCut);
		glUniform1f(mod.programPrincipal.getLocation("outerCut"), outerCut);

		glUniform3f(mod.programPrincipal.getLocation("dirLight.direction"), ejeXL, ejeYL, ejeZL);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.position"), direccionLuz[0], direccionLuz[1], direccionLuz[2]);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.ambient"), color_luz_ambiental[0], color_luz_ambiental[1], color_luz_ambiental[2]);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.diffuse"), color_luz_difusa[0], color_luz_difusa[1], color_luz_difusa[2]);
		glUniform3f(mod.programPrincipal.getLocation("dirLight.specular"), color_luz_especular[0], color_luz_especular[1], color_luz_especular[2]);

		glUniform3f(mod.programPrincipal.getLocation("model.ambient"), models[i].color_ambiental[0], models[i].color_ambiental[1], models[i].color_ambiental[2]);
		glUniform3f(mod.programPrincipal.getLocation("model.diffuse"), models[i].color_difuso[0], models[i].color_difuso[1], models[i].color_difuso[2]);
		glUniform3f(mod.programPrincipal.getLocation("model.specular"), models[i].color_especular[0], models[i].color_especular[1], models[i].color_especular[2]);

		glUniform1f(mod.programPrincipal.getLocation("model.shinyP_M"), models[i].shinyP_M);
		glUniform1f(mod.programPrincipal.getLocation("model.shinyBP_M"), models[i].shinyBP_M);
		glUniform1f(mod.programPrincipal.getLocation("model.fresnel_M"), models[i].fresnel_M);
		glUniform1f(mod.programPrincipal.getLocation("model.albido_M"), models[i].albido_M);
		glUniform1f(mod.programPrincipal.getLocation("model.roughnessOren_M"), models[i].roughnessOren_M);
		glUniform1f(mod.programPrincipal.getLocation("model.roughness_M"), models[i].roughness_M);
		glUniform1i(mod.programPrincipal.getLocation("model.currentSpecular_M"), models[i].currentSpecular_M);
		glUniform1i(mod.programPrincipal.getLocation("model.currentDifuso_M"), models[i].currentDifuso_M);


		glUniform1i(mod.programPrincipal.getLocation("hayLuz"), apagarLuz);


		//Matrices de view y proyeccion
		glm::mat4 model_mat;
		glm::vec3 norm(0.0f, 0.0f, 0.0f);
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		view_mat = glm::lookAt(eye, norm, up);
		view_mat = camera.GetViewMatrix();
		gluLookAt(eye[0], eye[1], eye[2], norm[0], norm[1], norm[2], up[0], up[1], up[2]);



		model_mat = translate_en_matriz(models[i].ejeX, models[i].ejeY, models[i].ejeZ);
		model_mat = model_mat * rotacion_en_matriz(models[i].rotacion[0], models[i].rotacion[1], models[i].rotacion[2], models[i].rotacion[3]);
		model_mat = model_mat * scale_en_matriz(models[i].scaleT);

		glUniformMatrix4fv(model_matr_loc, 1, GL_FALSE, glm::value_ptr(model_mat));
		glUniformMatrix4fv(view_matr_loc, 1, GL_FALSE, glm::value_ptr(view_mat));

		project_mat = glm::perspective(camera.Zoom, (float)GLFW_WIDTH / (float)GLFW_HEIGHT, 0.1f, 1000.0f);

		glUniformMatrix4fv(proj_matr_loc, 1, GL_FALSE, glm::value_ptr(project_mat));

		glEnable(GL_TEXTURE_2D);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


		if (i == 0 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture1);
		else if (i == 1 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture2);
		else if (i == 2 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture3);
		else if (i == 3 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture4);
		else if (i == 4 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture5);
		else if (i == 5 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture6);
		else if (i == 6 && models[i].texturizado) glBindTexture(GL_TEXTURE_2D, mod.texture7);
		//else if (i == 7) glBindTexture(GL_TEXTURE_2D, texture1);

		glBindBuffer(GL_ARRAY_BUFFER, models[i].vbo);
		//VERTICE Y NORMALES
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(models[i].vertices.size() * sizeof(float) + (models[i].coord_texturas.size() * sizeof(float))));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUniform1i(mod.programPrincipal.getLocation("position_i"), i);

		if (!models[i].texturizado) glUniform1i(mod.programPrincipal.getLocation("hayTex"), selectedModel);
		else  glUniform1i(mod.programPrincipal.getLocation("hayTex"), -1);

		glDrawArrays(GL_TRIANGLES, 0, models[i].vertices.size() / 3);


		glDisableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		mod.programPrincipal.disable();
		glPushMatrix();

		if (models[i].autoRotacion) {
			float eje[3] = { 0, 1, 0 };
			float angulo = (float)(getTimeMs() - tiempoRotacion) / 1000.0f;
			float quat[4];
			setQuat(eje, angulo, quat);
			multiplicarQuat(rotacionInicial, quat, rotacion);
			models[i].rotacion[0] = rotacion[0];
			models[i].rotacion[1] = rotacion[1];
			models[i].rotacion[2] = rotacion[2];
			models[i].rotacion[3] = rotacion[3];
		}

		if (models[i].autoTranslate) {
			if (i == 2) {
				if (tiempo_traslacion[0] > 99) tiempo_traslacion[0] = 0;
				if (tiempo_traslacion[0] >= 0 && tiempo_traslacion[0] < 50) {
					models[i].ejeX += 0.009;
					tiempo_traslacion[0]++;
				}
				else if (tiempo_traslacion[0] >= 50 && tiempo_traslacion[0] < 100) {
					models[i].ejeX -= 0.009;
					tiempo_traslacion[0]++;
				}
			}
			else {
				if (tiempo_traslacion[1] > 99) tiempo_traslacion[1] = 0;
				if (tiempo_traslacion[1] >= 0 && tiempo_traslacion[1] < 50) {
					models[i].ejeX += 0.009;
					tiempo_traslacion[1]++;
				}
				else if (tiempo_traslacion[1] >= 50 && tiempo_traslacion[1] < 100) {
					models[i].ejeX -= 0.009;
					tiempo_traslacion[1]++;
				}
			}
		}

		if (models[i].autoScale) {
			if (i == 6) {
				if (tiempo_escalar[0] > 99) tiempo_escalar[0] = 0;
				if (tiempo_escalar[0] >= 0 && tiempo_escalar[0] < 50) {
					models[i].scaleT += 0.01;
					tiempo_escalar[0]++;
				}
				else if (tiempo_escalar[0] >= 50 && tiempo_escalar[0] < 100) {
					models[i].scaleT -= 0.01;
					tiempo_escalar[0]++;
				}
			}
			else {
				if (tiempo_escalar[1] > 99) tiempo_escalar[1] = 0;
				if (tiempo_escalar[1] >= 0 && tiempo_escalar[1] < 50) {
					models[i].scaleT += 0.01;
					tiempo_escalar[1]++;
				}
				else if (tiempo_escalar[1] >= 50 && tiempo_escalar[1] < 100) {
					models[i].scaleT -= 0.01;
					tiempo_escalar[1]++;
				}
			}
		}

		if (selecting && i == selectedModel && i != 7) {

			draw_bb(bool_loc);

			models[selectedModel].shinyBP_M = shininessBP;
			models[selectedModel].shinyP_M = shininessP;
			models[selectedModel].fresnel_M = fresnel;
			models[selectedModel].albido_M = albido;
			models[selectedModel].roughnessOren_M = roughnessOren;
			models[selectedModel].roughness_M = roughness;
			models[selectedModel].currentDifuso_M = currentDifuso;
			models[selectedModel].currentSpecular_M = currentSpecular;

			models[selectedModel].color_ambiental[0] = color_mat_ambiental[0];
			models[selectedModel].color_ambiental[1] = color_mat_ambiental[1];
			models[selectedModel].color_ambiental[2] = color_mat_ambiental[2];

			models[selectedModel].color_difuso[0] = color_mat_difusa[0];
			models[selectedModel].color_difuso[1] = color_mat_difusa[1];
			models[selectedModel].color_difuso[2] = color_mat_difusa[2];

			models[selectedModel].color_especular[0] = color_mat_especular[0];
			models[selectedModel].color_especular[1] = color_mat_especular[1];
			models[selectedModel].color_especular[2] = color_mat_especular[2];

			if (selectedModel == 5 ) {
				models[selectedModel].scaleT = scaleT;

				models[selectedModel].ejeX = ejeX;
				models[selectedModel].ejeY = ejeY;
				models[selectedModel].ejeZ = ejeZ;

				models[selectedModel].rotacion[0] = rotacion_principal[0];
				models[selectedModel].rotacion[1] = rotacion_principal[1];
				models[selectedModel].rotacion[2] = rotacion_principal[2];
				models[selectedModel].rotacion[3] = rotacion_principal[3];
			}


		}

		if (i == 7) {
			if (selecting && selectedModel == 7) {
				draw_bb(bool_loc);
			}
			models[i].ejeX = ejeXL;
			models[i].ejeY = ejeYL;
			models[i].ejeZ = ejeZL;
			scaleT = models[selectedModel].scaleT;
		}

		glPopMatrix();
	}
}

//Funcion que dibuja el skybox
void draw_skybox() {
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	sky.programSkyBox.enable();

	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	glm::mat4 projection = glm::perspective(45.0f, (float)GLFW_WIDTH / (float)GLFW_HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(sky.programSkyBox.getLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv((sky.programSkyBox.getLocation("projection")), 1, GL_FALSE, glm::value_ptr(projection));
	// Cubo del Skybox
	glBindVertexArray(sky.skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(sky.programSkyBox.getLocation("skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, sky.cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	sky.programSkyBox.disable();
}

//Funcion que mueve la camara con WASD
void moverme() {
	bool una_vez = true;
	if ((keys['w'] || keys['W']) && una_vez) {
		//Muevo hacia delante
		camera.ProcessKeyboard(FORWARD, deltaTime);
		una_vez = false;
	}
	if ((keys['s'] || keys['S']) && una_vez) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		una_vez = false;
	}
	if ((keys['a'] || keys['A']) && una_vez) {
		camera.ProcessKeyboard(LEFT, deltaTime);
		una_vez = false;
	}
	if ((keys['d'] || keys['D']) && una_vez) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
		una_vez = false;
	}
}

//<-----------------FUNCIONES DE DESPLIEGUE--------------------->

//Funcion de display (llama a draw, crea skybox)
void display() {
	GLfloat currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame; 
	lastFrame = currentFrame;
	draw_skybox(); 	// Dibujo skybox de primero
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glClear(GL_STENCIL_BUFFER_BIT);
	draw(); 	// Dibujo todos los modelos
	moverme();
	TwDraw();
	glDisable(GL_STENCIL_TEST);
}


//<-----------------FUNCION DE MAIN--------------------->
int main(int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);


	GLFWwindow *window = glfwCreateWindow(GLFW_WIDTH, GLFW_HEIGHT, "Tarea 1 FCG - Alejandro Barone", nullptr, nullptr);
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
		
	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, inter.reshape);
	glfwSetKeyCallback(window, inter.keyFunc);
	glfwSetMouseButtonCallback(window, inter.motionFunc);
	glfwSetCursorPosCallback(window, inter.passiveMotionFunc);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCharCallback(window, (GLFWcharfun)inter.obtenerTecla);
	//glfwSetScrollCallback(window, scroll); SERIA EXTRA (ESCALAR CON SCROLL?)

	inter.initAntTweakParam();

	initGlew();
	glewExperimental = GL_TRUE;
	
	if (GLEW_OK != glewInit()) {
		cout << "ERROR AL INICIALIZAR GLEW" << endl;
		return EXIT_FAILURE;
	}
	mod.loadShaderPrincipal();
	sky.loadShaderSkyBox(); //cargo shader skybox
	sky.initSkybox(); //inicializado el skybox


	//Todos los objetos
	read_obj("Modelos/obj/floor.obj");
	read_obj("Modelos/obj/AlienPlanet2.obj");
	read_obj("Modelos/obj/cy.obj");
	read_obj("Modelos/obj/cyborg.obj");
	read_obj("Modelos/obj/MarsPlanet.obj");

	read_obj("Modelos/obj/bad_skull.obj");
	read_obj("Modelos/obj/arrow.obj");
	read_obj("Modelos/obj/MarsPlanet.obj"); //SOL

	//Habilitamos el alfa de los colores
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	inter.reshape(window, GLFW_WIDTH, GLFW_HEIGHT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	tiempoRotacion = getTimeMs();
	while (!glfwWindowShouldClose(window)) {
		display();
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}