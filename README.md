# Tarea 1 - [FCG]


## Contenido

* [Enunciado](#enunciado)
* [Mundo](#mundo)
* [Uso](#uso)
* [Herramienta](#herramienta)
* [Instalacion](#instalacion)
* [Autor](#autor)


#Enunciado

El enunciado de la tarea se encuentra en el pdf **_Enunciado.pdf_**

# Mundo

El mundo esta formado por 8 modelos 3D contando plano y esfera solar. El modelo principal del mundo es la **Calavera**, este es el unico modelo que se puede escalar/rotar/trasladar de manera libre, todos los demas objetos (a excepcion de la esfera solar y el plano) tienen una animacion asociada. En cada caso usted puede deshabilitar las animaciones de los modelos no principales. Para la esfera solar se tiene una traslacion libre en todos los ejes.

Ademas se tiene un skybox espacial para acompañar la escena

A continuacion una imagen de dicho mundo:
![alt tag](https://i.gyazo.com/8ba89bc026010a5d36a26adcb87ec13c.png)

# Uso

Al cargar el mundo usted podra moverse libremente por la escena usando las teclas de su teclado W (arriba), S (abajo), A(izquierda) y D (derecha). Si desea seleccionar un modelo debe entrar en el "modo seleccion", eso se logra tecleando la tecla "M" de su teclado, vera que el cursor aparecera en la pantalla y esta no se movera, permitiendole escoger el modelo que desee. Todos los objetos del mundo se seleccionan con el picking del mouse (a excepcion del plano, este se hizo no clickeable). Segun el objeto que seleccione el menu cambiara, por ejemplo, si selecciona al modelo principal saldra su menu correspondiente, asi como a los modelos no principales y a la luz. Para des-seleccionar un modelo solamente le debe **dar click nuevamente** a dicho objeto que ya esta seleccionado, o en cuyo caso seleccionar otro.

Todos los modelos cuentan con colores ambientales, difuso y especular del material, porder activar/desactivar texturas asi como el modelo difusa y especular para cada modelo por separado. En caso de que usted requiera editar valores de un modelo especular o difuso solamente seleccione el modelo y seleccione el boton **"Editar"**, se le abrira una ventana con los parametros a editar de dicho modelo.

Para la luz (esfera solar) podra editar que tipo de iluminacion sera (Puntual, Direccional o Reflector) y asi como en los modelos difuso y especular para editar sus campos le debe dar click al boton de **"Editar"**. Cabe destacar que la direccion del spot tambien se edita en esta nueva ventana. Tambien se tiene un boton para activar/desactivar la luz y la edicion de los colores de la luz difuso, especular y ambiental.

Las funcionalidad de interpolacion (Goraoud y Phong) solo se muestra en el menu principal -cuando no se esta seleccionando ningun modelo-. 


# Herramienta 

La tarea previamente explicada fue desarrollada usando las siguientes herramientas

| Herramienta                         	 | Versión   													   |                            
|----------------------------------------|-----------------------------------------------------------------|
| Microsoft Visual Studio        	 	 | 2015      													   |
|----------------------------------------|-----------------------------------------------------------------|
| OpenGL				        	 	 | 4.0.0      													   |
|----------------------------------------|-----------------------------------------------------------------|
| GLSL Version				             | 4.00      													   |

Y fue probada en la siguiente GPU:


| Fabricante                         	 | Nombre   													   |                            
|----------------------------------------|-----------------------------------------------------------------|
| Intel     	 					     | Intel HD Graphics 4000      									   |



# Instalacion

Para usar el proyecto se debe primero clonar el repositorio con el siguiente comando:

> git clone https://github.com/alejandrombc/FCG_Tarea1.git

Con esto puede ejecutar el ejecutable en la carpeta _/bin_ o abrir el proyecto de VS ubicado en _Tarea 1 - 24206267_FCG_ con extension **.sln**.


# Autor

**Alejandro Barone**
**CI: 24206267**
