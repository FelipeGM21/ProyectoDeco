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
#include <map>
#include <string>
#include <vector>

using namespace std;

enum {
	basex,
	cuerpoinferior,
	cuerposuperior,
	brazo
};

class baldosa {
public:
	enum posicion {
		suelo = 0,
		pared1 = 1,
		pared2 = 2,
		null = 3,
		mueble = 4,
		cuadro = 5
	};
	posicion estado;
	int posX;
	int posY;
	int posZ;

	baldosa() {
		estado = baldosa::null;
		posX = -1;
		posY = -1;
		posZ = -1;
	}
	baldosa(posicion status, int x, int y, int z) {
		estado = status;
		posX = x;
		posY = y;
		posZ = z;
	}
	posicion get_estado() {
		return estado;
	}
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
	int altura;
	int ancho;
	int largo;
	int ***matrizOcupacion;
	GLubyte colores[32][32][32][3];
	int conteoColor;
	int numeroFiguras;
	int figurasDisponibles;
	int figurasColocadas;
	vector<int> arrayFiguras;
	vector<vector<int>> arrayPosiciones;
	int figuras[256];
	int posiciones[256][3];
	map<int, baldosa> mapa_baldosas;
	int x_seleccionada;
	int y_seleccionada;
	int z_seleccionada;
	baldosa::posicion estado_selec;

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
	GLuint get_number(GLubyte const* ptr) {
		// magic numbers for shift sizes are safe because GL types have fixed sizes
		// (8 for GLubyte and 32 for GLuint)
		return (ptr[0] << 16) + (ptr[1] << 8) + ptr[2];
	}
	void get_bytes(GLuint number, GLubyte* out) {
		out[0] = (number & 0xFF0000) >> 16;
		out[1] = (number & 0x00FF00) >> 8;
		out[2] = number & 0x0000FF;
	}

	// método con las llamadas OpenGL para visualizar la escena
	void visualizar(void);

	void visualizar_colores();

	///// Apartado B: Métodos para visualizar cada parte del modelo
	void cuerpo();
	void cabeza();
	void brazo();
	void antebrazo();
	void muslo();
	void pierna();
	void pintarFigura(int numFigura);
	void pintarFiguraC(GLubyte color[3], int numFigura);
	void pintar_quad(float div_x, float div_z);

	void crearSuelo(int tamx, int tamz, int y);

	void crearSueloB(int tamx, int tamz, int y, baldosa::posicion estado);

	void crearParedX(int alto, int largo, int z);

	void crearParedZ(int alto, int largo, int z);

	void crearSueloBC(int tamx, int tamz, int y, baldosa::posicion estado);

	void crearParedXBC(int alto, int largo, int z, baldosa::posicion estado);

	void crearParedZBC(int alto, int profundo, int x, baldosa::posicion estado);

	void crearParedXB(int alto, int largo, int z, baldosa::posicion estado);

	void crearParedZB(int alto, int profundo, int x, baldosa::posicion estado);

	void crearMuebles(int tecla);

	void borrarMuebles();


	////// Apartado D: añadir aquí los métodos para modificar los grados de libertad del modelo
	void cambiarParte(int eje, int incremento);

	////// Apartado E: añadir aquí el método/s para indicar el objeto seleccionado
	void seleccionar(int parte);

	bool get_ejes() { return ejes; };
	void set_ejes(bool _ejes) { ejes = _ejes; };

	void seleccion_color(GLubyte color[3]);

};

#endif
