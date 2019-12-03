#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif
#include "igvFuenteLuz.h"
#include "igvMallaTriangulos.h"

enum {
	basex,
	cuerpoinferior,
	cuerposuperior,
	brazo
};

class igvEscena3D {
	static const int ejex = 0;
	static const int ejey = 1;
	static const int ejez = 2;
	GLfloat gris[3] = { 0.5,0,0 };
	GLfloat amarillo[3] = { 1,1,0 };
	bool practica[4];
	igvColor kAmbiental;
	igvColor kDifuso;
	igvColor kSpectacular;
	double nPhong;
	igvPunto3D puntoFoco;
	igvMallaTriangulos *malla;
	protected:
////// Apartado D: añadir quí los atributos para el control de los grados de libertad del modelo
		int matrizRotaciones[9][3];

////// Apartado E: añadir aquí el atributo/s para el control del objeto seleccionado y colorearlo en amarillo
		int parteSeleccionada;

		// Otros atributos		
		bool ejes;
		bool bv;

	public:
////// Apartado E: color de los volúmenes envolventes para la selección
		

		// Constructores por defecto y destructor
		igvEscena3D();
		~igvEscena3D();

		// Metodos estáticos

////// Apartado C: Métodos para visualizar los volúmenes envolventes (BV) 
    void pintar_BV(float escalaX, float escalaY, float escalaZ, GLubyte *color);
	void pintarFiguraBV();
	void visualizar_BV();
	void activar_BV();
	
	// método con las llamadas OpenGL para visualizar la escena
    void visualizar(void);

///// Apartado B: Métodos para visualizar cada parte del modelo
	void cuerpo();
	void cabeza();
	void brazo();
	void antebrazo();
	void muslo();
	void pierna();
	void pintarFigura();


////// Apartado D: añadir aquí los métodos para modificar los grados de libertad del modelo
	void cambiarParte(int eje, int incremento);

////// Apartado E: añadir aquí el método/s para indicar el objeto seleccionado
	void seleccionar(int parte);

	bool get_ejes() { return ejes; };
	void set_ejes(bool _ejes) { ejes = _ejes; };

};

#endif
