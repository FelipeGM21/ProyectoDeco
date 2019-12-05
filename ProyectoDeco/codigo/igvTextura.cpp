#include "igvTextura.h"

// Metodos constructores y destructor

igvTextura::igvTextura(char *fichero) {
  GLubyte *imagen;

	imagen = loadBMPRaw(fichero, alto, ancho);

	/* Apartado G: Añadir aquí el código para cargar como textura OpenGL el BMP imagen */
  /*
	- Generar el identificador de textura y asignarlo al atributo idTextura
	- Enlazar el identificador creado a GL_TEXTURE_2D
  - Especificar la textura, asignádole como textura el array imagen
  - Modo de aplicación de la textura
	- Parámetros de la textura: repetición y filtros
	*/
	GLuint id = idTextura;
	glEnable(GL_TEXTURE_2D);
	//glGenTextures(1, id);
	glBindTexture(GL_TEXTURE_2D, idTextura);
	// -- Cambiar parámetros de textura --
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB,
		ancho, alto, 0, GL_RGB,
		GL_UNSIGNED_BYTE, imagen
	);



	delete imagen;
}

void igvTextura::aplicar(void) {
  glBindTexture(GL_TEXTURE_2D, idTextura);
}

igvTextura::~igvTextura() {
  glDeleteTextures(1, &idTextura); 
}
