#pragma once
//Alejandro Barone - CI 24206267

#pragma once
#include <GL/glew.h> //Incluida desde carpeta (glew)
#include <GLFW/glfw3.h>
#include <AntTweakBar.h>
#include <vector>
#include <windows.h>
#include <fstream>
#include <sstream>
#include <ctime> 
#include <algorithm>
#include <iostream>
#include <glm/common.hpp> //Incluida desde carpeta (glm)
#include <glm/gtc/matrix_transform.hpp> //Incluida desde carpeta (glm)
#include <glm/gtc/type_ptr.hpp> //Incluida desde carpeta (glm)
#include <IL/il.h> //Incluida desde carpeta (Texturas)
#include "Camera.h"
#include "GLSLProgram.h"

//Para las normales y su offset
#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

using namespace std;

typedef enum { Gouraud = 1, Phong } interpolacion;
typedef enum { BlinnPhong = 1, PhongS, CookTorrance } especular;
typedef enum { Lambert = 1, OrenNayar } difuso;
typedef enum { Direccional = 1, Puntual, Reflector } lightT;