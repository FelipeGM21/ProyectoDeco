#include "igvTextura.h"

// Metodos constructores y destructor

igvTextura::igvTextura(const char *fichero) {
  GLubyte *imagen;

	//imagen = loadBMPRaw(fichero, alto, ancho);
	/* Apartado G: A�adir aqu� el c�digo para cargar como textura OpenGL el BMP imagen */
  /*
	- Generar el identificador de textura y asignarlo al atributo idTextura
	- Enlazar el identificador creado a GL_TEXTURE_2D
  - Especificar la textura, asign�dole como textura el array imagen
  - Modo de aplicaci�n de la textura
	- Par�metros de la textura: repetici�n y filtros
	*/
	//Generar el identificador de textura y asignarlo al atributo idTextura
	glGenTextures(1, &idTextura);
	//Enlazar el identificador creado a GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, idTextura);
	//Especificar la textura, asign�dole como textura el array imagen
	/*
	glTexImage2D(
		GL_TEXTURE_2D, 0, 3,
		ancho, alto, 0, GL_RGB,
		GL_UNSIGNED_BYTE, imagen
	);*/
	//  - Modo de aplicaci�n de la textura
		//-Par�metros de la textura : repetici�n y filtros
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Pegado sobre el pol�gono
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Filtros
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	//Cargamos un fichero bmp en el array
	imagen= loadBMPRaw(fichero, alto, ancho);

	glTexImage2D(
		GL_TEXTURE_2D, 0, 3,
		ancho, alto, 0, GL_RGB,
		GL_UNSIGNED_BYTE, imagen
	);

	//Activamos las texturas
	glEnable(GL_TEXTURE_2D);
}

void igvTextura::aplicar(void) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, idTextura);
}

igvTextura::~igvTextura() {
  glDeleteTextures(1, &idTextura); 
}
