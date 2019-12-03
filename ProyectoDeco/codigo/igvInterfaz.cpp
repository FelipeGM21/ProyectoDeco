#include <cstdlib>
#include <stdio.h>

#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
                             // ellos a las variables de la clase

// Metodos constructores -----------------------------------

igvInterfaz::igvInterfaz() :tipoCam(IGV_PARALELA), p0(3.0, 2.0, 4), r(0, 0, 0), v(0, 1.0, 0),
xwmin(-1 * 3), xwmax(1 * 3), ywmin(-1 * 3), ywmax(1 * 3), znear(1.0), zfar(100), angulo(60.0), raspecto(1.0),
tipoPerspectiva(0), factor(1.0), formato169(false), pos_x(0), pos_y(0) {
	//// Apartado E: inicialización de los atributos para realizar la selección
	modo = IGV_VISUALIZAR;
	objeto_seleccionado = -1;
	boton_retenido = false;

	//Perspectiva
	perspectivas[0][0] = p0;
	perspectivas[1][0] = r;
	perspectivas[2][0] = v;
	//Alzado
	perspectivas[0][1] = { 3.0,0.0,0 };
	perspectivas[1][1] = r;
	perspectivas[2][1] = { 0,1.0,0 };
	//Planta
	perspectivas[0][2] = { 0,3.0,0 };
	perspectivas[1][2] = r;
	perspectivas[2][2] = { 1.0,0,0 };
	//Perfil
	perspectivas[0][3] = { 0,0,3.0 };
	perspectivas[1][3] = r;
	perspectivas[2][3] = { 0,1.0,0 };
}
igvInterfaz::~igvInterfaz () {}


// Metodos publicos ----------------------------------------

void igvInterfaz::crear_mundo(void) {
	// crear cámaras
	interfaz.camara.set(IGV_PARALELA, igvPunto3D(3.0,2.0,4),igvPunto3D(0,0,0),igvPunto3D(0,1.0,0),
		                                -1*4.5, 1*4.5, -1*4.5, 1*4.5, -1*3, 200);
}

void igvInterfaz::configura_entorno(int argc, char** argv,
			                              int _ancho_ventana, int _alto_ventana,
			                              int _pos_X, int _pos_Y,
													          string _titulo){
	// inicialización de las variables de la interfaz																	
	ancho_ventana = _ancho_ventana;
	alto_ventana = _alto_ventana;

	// inicialización de la ventana de visualización
	glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(_ancho_ventana,_alto_ventana);
  glutInitWindowPosition(_pos_X,_pos_Y);
	glutCreateWindow(_titulo.c_str());

	glEnable(GL_DEPTH_TEST); // activa el ocultamiento de superficies por z-buffer
  glClearColor(1.0,1.0,1.0,0.0); // establece el color de fondo de la ventana

	glEnable(GL_LIGHTING); // activa la iluminacion de la escena
  glEnable(GL_NORMALIZE); // normaliza los vectores normales para calculo iluminacion

	glEnable(GL_TEXTURE_2D); // activa el uso de texturas

	crear_mundo(); // crea el mundo a visualizar en la ventana
}

void igvInterfaz::inicia_bucle_visualizacion() {
	glutMainLoop(); // inicia el bucle de visualizacion de OpenGL
}

void igvInterfaz::set_glutSpecialFunc(int key, int x, int y) {
	// Apartado F: manejo de las teclas especiales del teclado para mover la posición del foco


	glutPostRedisplay(); // renueva el contenido de la ventana de vision
}

void igvInterfaz::set_glutKeyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'w': interfaz.p0[2] += 0.1;
//		interfaz.perspectivas[0][0][2] += 0.1;
		interfaz.r[2] += 0.1;
		break;
	case 'a': interfaz.p0[0] += 0.1;
		interfaz.r[0] += 0.1;
		break;
	case 's': interfaz.p0[2] -= 0.1;
		interfaz.r[2] -= 0.1;
		break;
	case 'd': interfaz.p0[0] -= 0.1;
		interfaz.r[0] -= 0.1;
		break;
	case 'q': interfaz.p0[1] -= 0.1;
		break;
	case 'r': interfaz.p0[1] += 0.1;
		break;
	case 'p': // cambia el tipo de proyección de paralela a perspectiva y viceversa
		if (interfaz.tipoCam == IGV_PARALELA) {
			interfaz.tipoCam = IGV_PERSPECTIVA;
		}
		else {
			interfaz.tipoCam = IGV_PARALELA;
		}
		break;
	case '+': // zoom in
		interfaz.factor *= 0.95;
		break;
	case '-': // zoom out
		interfaz.factor *= 1.05;
		break;
	case 'c': // incrementar la distancia del plano cercano
		interfaz.znear += 0.2;
		break;
	case 'C': // decrementar la distancia del plano cercano
		interfaz.znear -= 0.2;
		break;
	//case '9': // cambiar a formato 16:9 con deformación
	//	if (interfaz.formato169) {
	//		interfaz.pos_x = 0;
	//		interfaz.pos_y = 0;
	//		interfaz.ancho_window = interfaz.ancho_ventana;
	//		interfaz.alto_ventana = interfaz.alto_ventana;
	//		interfaz.formato169 = false;
	//	}
	//	else {
	//		interfaz.pos_x = 0;
	//		interfaz.pos_y = ((interfaz.alto_ventana / 16) * 9) / 2;
	//		interfaz.ancho_window = interfaz.ancho_ventana;
	//		interfaz.alto_window = (interfaz.alto_ventana / 16) * 9;
	//		interfaz.formato169 = true;
	//	}
	//	//interfaz.set_glutDisplayFunc();
	//	break;
		////// Apartado D: incluir aquí el cambio de la cámara para mostrar las vistas planta, perfil, alzado o perspectiva
	case 'v': // cambia la posición de la cámara para mostrar las vistas planta, perfil, alzado o perspectiva
		interfaz.tipoPerspectiva = (interfaz.tipoPerspectiva + 1) % 4;
		interfaz.p0 = interfaz.perspectivas[0][interfaz.tipoPerspectiva];
		interfaz.r = interfaz.perspectivas[1][interfaz.tipoPerspectiva];
		interfaz.v = interfaz.perspectivas[2][interfaz.tipoPerspectiva];
		break;
		////// Apartado D: incluir aquí la modificación de los grados de libertad del modelo pulsando las teclas correspondientes
	case 'k': interfaz.objeto_seleccionado++;
		if (interfaz.objeto_seleccionado > 8) interfaz.objeto_seleccionado = 0;
		interfaz.escena.seleccionar(interfaz.objeto_seleccionado);
		break;
	case 'x': interfaz.escena.cambiarParte(ejex, 5);
		break;
	case 'X': interfaz.escena.cambiarParte(ejex, -5);
		break;
	case 'y': interfaz.escena.cambiarParte(ejey, 5);
		break;
	case 'Y': interfaz.escena.cambiarParte(ejey, -5);
		break;
	case 'z': interfaz.escena.cambiarParte(ejez, 5);
		break;
	case 'Z': interfaz.escena.cambiarParte(ejez, -5);
		break;
	case 'e': // activa/desactiva la visualizacion de los ejes
		interfaz.escena.set_ejes(interfaz.escena.get_ejes() ? false : true);
		break;
	case 27: // tecla de escape para SALIR
		exit(1);
		break;
	}
	if (interfaz.tipoCam == IGV_PARALELA) {
		interfaz.camara.set(interfaz.tipoCam,
			interfaz.p0, interfaz.r, interfaz.v,
			interfaz.xwmin*interfaz.factor, interfaz.xwmax*interfaz.factor, interfaz.ywmin*interfaz.factor, interfaz.ywmax*interfaz.factor, interfaz.znear, interfaz.zfar);
	}
	else {
		interfaz.camara.set(interfaz.tipoCam,
			interfaz.p0, interfaz.camara.r, interfaz.v,
			interfaz.angulo*interfaz.factor, interfaz.raspecto, interfaz.znear, interfaz.zfar);
	}
	interfaz.camara.aplicar();
	glutPostRedisplay(); // renueva el contenido de la ventana de vision
}

void igvInterfaz::set_glutReshapeFunc(int w, int h) {
  // dimensiona el viewport al nuevo ancho y alto de la ventana
  // guardamos valores nuevos de la ventana de visualizacion
  interfaz.set_ancho_ventana(w);
  interfaz.set_alto_ventana(h);

	// establece los parámetros de la cámara y de la proyección
	interfaz.camara.aplicar();
}

void igvInterfaz::set_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el z-buffer

	// se establece el viewport
	glViewport(0, 0, interfaz.get_ancho_ventana(), interfaz.get_alto_ventana());

	// establece los parámetros de la cámara y de la proyección
	interfaz.camara.aplicar();

	//visualiza la escena
	interfaz.escena.visualizar();

	// refresca la ventana
	glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo
}

void igvInterfaz::inicializa_callbacks() {
	glutKeyboardFunc(set_glutKeyboardFunc);
	glutReshapeFunc(set_glutReshapeFunc);
	glutDisplayFunc(set_glutDisplayFunc);
	glutSpecialFunc(set_glutSpecialFunc);
}
