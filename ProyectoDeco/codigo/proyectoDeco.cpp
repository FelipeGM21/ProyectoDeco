#include <cstdlib>

#include "igvInterfaz.h"

// Objetos globales
// objeto que contiene la configuración y la interacción con la ventana de visualización
igvInterfaz interfaz;

int main (int argc, char** argv) {
	// inicializa la ventana de visualización
	interfaz.configura_entorno(argc,argv,
		                         1200,720, // tamaño de la ventana
														 0,0, // posicion de la ventana
														 "Informática Gráfica y Visualización. Proyecto Decoracion de Interiores." // título de la ventana
														 );

	// establece las funciones callbacks para la gestión de los eventos
	interfaz.inicializa_callbacks();

	// inicia el bucle de visualización de OpenGL
	interfaz.inicia_bucle_visualizacion();

	return(0);
}
