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
#include "igvTextura.h"
#include "igvMaterial.h"
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
	GLfloat gris[3] = { 0.5,0,0 };
	GLfloat marron[3] = { 1.04f, 0.59f, 0.16f };
	GLfloat amarillo[3] = { 1,1,0 };
	static const int ejex = 0;
	static const int ejey = 1;
	static const int ejez = 2;
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
	map<int, baldosa> mapa_baldosas;
	int x_seleccionada;
	int y_seleccionada;
	int z_seleccionada;
	baldosa::posicion estado_selec;
	igvTextura pared;
	igvTextura pared_seleccion;
	igvTextura suelo;
	igvTextura suelo_seleccion;
	igvTextura madera;
	igvTextura madera_seleccion;
	//Texturas cuadros
	igvTextura marco;
	igvTextura marco_seleccion;
	igvTextura paisaje;
	igvTextura paisaje_seleccion;
	igvTextura fondo_parte1;
	igvTextura fondo_parte2;
	igvTextura fondo_parte1_seleccion;
	igvTextura fondo_parte2_seleccion;
	//Texturas planta
	igvTextura hojas;
	igvTextura hojas_seleccion;
	igvTextura palo;
	igvTextura palo_seleccion;
	igvTextura tierra_tex;
	igvTextura tierra_seleccion;
	igvTextura maceta_tex;
	igvTextura maceta_seleccion;

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

	//Metodos para crear muebles
	void cuerpo();
	void cabeza();
	void brazo();
	void antebrazo();
	void muslo();
	void pierna();
	void pintarFigura(int numFigura, int rota);
	void pintarFiguraC(GLubyte color[3], int numFigura, int rota);
	void pintar_quad(float div_x, float div_z);

	//moobiliario
	void silla(int numFigura);
	void sillaC(GLubyte color[3], int numFigura);
	void mesa(int numFigura);
	void mesaC(GLubyte color[3], int numFigura);
	void manzana();
	void cuadroH();
	void cuadroV();

	void pintar_quad(float div_x, float div_z,const char*archivo);
	void pintar_quad(float div_x, float div_z, igvTextura textura);
	void pintar_triangulo3D();
	void pintar_triangulo3D(const char *archivo);
	void pintar_triangulo3D(igvTextura archivo);
	void parte_cuadro();
	void parte_cuadro(const char *archivo);
	void parte_cuadro(igvTextura textura);
	void parte_cuadro2(igvTextura textura);
	void cuadro(int numFigura);
	void cuadroC(GLubyte color[3], int numFigura);
	void cuadroV(int numFigura);
	void cuadroVC(GLubyte color[3], int numFigura);
	void planta(int numFigura);
	void plantaC(GLubyte color[3], int numFigura);
	void tierra(igvTextura textura);
	void tierra();
	void maceta(igvTextura textura);
	void maceta();
	void crearSuelo(int x, int y);
	void crearPared(int largo, int alto);

	//Metodos para crear suelo y paredes
	void crearSueloB(int tamx, int tamz, int y, baldosa::posicion estado);
	void crearParedXB(int alto, int largo, int z, baldosa::posicion estado);
	void crearParedZB(int alto, int profundo, int x, baldosa::posicion estado);

	//Metodos para crear suelo y paredes para seleccion de color
	void crearSueloBC(int tamx, int tamz, int y, baldosa::posicion estado);
	void crearParedXBC(int alto, int largo, int z, baldosa::posicion estado);
	void crearParedZBC(int alto, int profundo, int x, baldosa::posicion estado);

	//Metodos para actualizar muebles
	void crearMuebles(int tecla);
	void rotarFigura();
	void borrarMuebles();
	void actualizarOcupacion();


	////// Apartado D: añadir aquí los métodos para modificar los grados de libertad del modelo
	void cambiarParte(int eje, int incremento);

	////// Apartado E: añadir aquí el método/s para indicar el objeto seleccionado
	void seleccionar(int parte);

	bool get_ejes() { return ejes; };
	void set_ejes(bool _ejes) { ejes = _ejes; };

	void seleccion_color(GLubyte color[3]);

};

#endif
