#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <string>

#include "igvEscena3D.h"
#include "igvCamara.h"

using namespace std;
typedef enum {
	IGV_VISUALIZAR,
	IGV_SELECCIONAR,
} modoInterfaz;

class igvInterfaz {
	protected:
		// Atributos
		// Atributos
		int ancho_ventana; // ancho inicial de la ventana de visualizacion
		int alto_ventana;  // alto inicial de la ventana de visualizacion
		int ancho_window;
		int alto_window;
		static const int ejex = 0;
		static const int ejey = 1;
		static const int ejez = 2;
		int time_now;
		int lastTick = 0;

		igvEscena3D escena; // escena que se visualiza en la ventana definida por igvInterfaz
		igvCamara camara; // c�mara que se utiliza para visualizar la escena

		// Parametros de camara
		int pos_x;
		int pos_y;
		tipoCamara tipoCam;
		igvPunto3D p0;
		igvPunto3D r;
		igvPunto3D v;
		double xwmin, xwmax, ywmin, ywmax, znear, zfar;
		double angulo, raspecto;
		igvPunto3D perspectivas[3][4];
		int tipoPerspectiva;
		float factor;
		bool formato169;

		///// Apartado E: atributos para la selecci�n mediante el rat�n
		modoInterfaz modo; // IGV_VISUALIZAR: en la ventana se va a visualizar de manera normal la escena, 
						   // IGV_SELECCIONAR: se ha pulsado sobre la ventana de visualizaci�n, la escena se debe
						   // visualizar en modo selecci�n
		int cursorX, cursorY; // pixel de la pantalla sobre el que est� situado el rat�n, por pulsar o arrastrar

		int objeto_seleccionado; // c�digo del objeto seleccionado, -1 si no hay ninguno

		bool boton_retenido; // indica si el bot�n est� pulsado (true) o se ha soltado (false)
	///// Apartado G: atributos para la animaci�n
		int movimiento = 0;
		int llegaFinal = 90;
		int incremento = 5;
		bool animacion = false;

		///// Apartado D: atributos para la visualizaci�n de los vol�menes envolventes


	public:
		// Constructores por defecto y destructor
		igvInterfaz();
		~igvInterfaz();

		// Metodos est�ticos
		// callbacks de eventos
		static void set_glutKeyboardFunc(unsigned char key, int x, int y); // metodo para control de eventos del teclado
		static void set_glutSpecialFunc(int key, int x, int y); // metodo para control de eventos del teclado (teclas especiales)
		static void set_glutReshapeFunc(int w, int h); // m�todo que define la camara de vision y el viewport
		                                               // se llama autom�ticamente cuano se camba el tama�o de la ventana
		static void set_glutDisplayFunc(); // m�todo para visualizar la escena


		// Metodos
		// crea el mundo que se visualiza en la ventana
		void crear_mundo(void);

		// inicializa todos los par�metros para crear una ventana de visualizaci�n
		void configura_entorno(int argc, char** argv, // parametros del main
			                     int _ancho_ventana, int _alto_ventana, // ancho y alto de la ventana de visualizaci�n
			                     int _pos_X, int _pos_Y, // posicion inicial de la ventana de visualizaci�n
													 string _titulo // t�tulo de la ventan de visualizaci�n
													 );
		void inicializa_callbacks(); // inicializa todas los callbacks

		void inicia_bucle_visualizacion(); // visualiza la escena y espera a eventos sobre la interfaz

		// m�todos get_ y set_ de acceso a los atributos
		int get_ancho_ventana(){return ancho_window;};
		int get_alto_ventana(){return alto_window;};

		void set_ancho_ventana(int _ancho_ventana){ancho_window = _ancho_ventana;};
		void set_alto_ventana(int _alto_ventana){alto_window = _alto_ventana;};
};

#endif
