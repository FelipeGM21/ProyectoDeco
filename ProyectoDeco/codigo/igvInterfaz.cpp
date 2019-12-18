#include <cstdlib>
#include <stdio.h>

#include "igvInterfaz.h"

extern igvInterfaz interfaz; // los callbacks deben ser estaticos y se requiere este objeto para acceder desde
							 // ellos a las variables de la clase

// Metodos constructores -----------------------------------

igvInterfaz::igvInterfaz() :tipoCam(IGV_PERSPECTIVA), posicion(3.0, 2.0, 4), centro(-3.0, -2.0, -4), vector_up(0, 1.0, 0),
xwmin(-1 * 3), xwmax(1 * 3), ywmin(-1 * 3), ywmax(1 * 3), znear(0.1), zfar(100), angulo(60.0), raspecto(1.0),
tipoPerspectiva(0), factor(1.0), formato169(false), pos_x(0), pos_y(0), pitch(-0.4), yaw(-2.3), cursorX(INT_MAX), cursorY(INT_MAX) {
	//// Apartado E: inicialización de los atributos para realizar la selección
	modo = IGV_VISUALIZAR;
	objeto_seleccionado = -1;
	boton_retenido = false;

	//Perspectiva
	perspectivas[0][0] = { 3.0, 2.0, 4 };
	perspectivas[1][0] = centro;
	perspectivas[2][0] = { 0, 1.0, 0 };
	//Alzado
	perspectivas[0][1] = { 3.0,1.5,1.5 };
	perspectivas[1][1] = { -1.0,0,0 };
	perspectivas[2][1] = { 0,1.0,0 };
	//Planta
	perspectivas[0][2] = { 1.5,4.0,1.5 };
	perspectivas[1][2] = { 0,-1.0,0 };
	perspectivas[2][2] = { 1.0,0,0 };
	//Perfil
	perspectivas[0][3] = { 1.5,1.5,3.0 };
	perspectivas[1][3] = { 0,0,-1.0 };
	perspectivas[2][3] = { 0,1.0,0 };
}
igvInterfaz::~igvInterfaz() {}


// Metodos publicos ----------------------------------------

void igvInterfaz::actualiza_camara(void) {
	interfaz.centro[0] = cos(interfaz.pitch) * cos(interfaz.yaw);
	interfaz.centro[1] = sin(interfaz.pitch);
	interfaz.centro[2] = cos(interfaz.pitch) * sin(interfaz.yaw);
	if (interfaz.tipoCam == IGV_PARALELA) {
		interfaz.camara.set(interfaz.tipoCam,
			interfaz.posicion, interfaz.centro, interfaz.vector_up,
			interfaz.xwmin*interfaz.factor, interfaz.xwmax*interfaz.factor, interfaz.ywmin*interfaz.factor, interfaz.ywmax*interfaz.factor, interfaz.znear, interfaz.zfar);
	}
	else {
		interfaz.camara.set(interfaz.tipoCam,
			interfaz.posicion, interfaz.centro, interfaz.vector_up,
			interfaz.angulo*interfaz.factor, interfaz.raspecto, interfaz.znear, interfaz.zfar);
	}
	interfaz.camara.aplicar();
}

void igvInterfaz::crear_mundo(void) {
	// crear cámaras
	interfaz.camara.set(interfaz.tipoCam,
		interfaz.posicion, interfaz.centro, interfaz.vector_up,
		interfaz.xwmin*interfaz.factor, interfaz.xwmax*interfaz.factor, interfaz.ywmin*interfaz.factor, interfaz.ywmax*interfaz.factor, interfaz.znear, interfaz.zfar);
	interfaz.camara.set(interfaz.tipoCam,
		interfaz.posicion, interfaz.camara.target, interfaz.vector_up,
		interfaz.angulo*interfaz.factor, interfaz.raspecto, interfaz.znear, interfaz.zfar);
	camara.aplicar();
}

void igvInterfaz::configura_entorno(int argc, char** argv,
	int _ancho_ventana, int _alto_ventana,
	int _pos_X, int _pos_Y,
	string _titulo) {
	// inicialización de las variables de la interfaz																	
	ancho_ventana = _ancho_ventana;
	alto_ventana = _alto_ventana;

	// inicialización de la ventana de visualización
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(_ancho_ventana, _alto_ventana);
	glutInitWindowPosition(_pos_X, _pos_Y);
	glutCreateWindow(_titulo.c_str());

	glEnable(GL_DEPTH_TEST); // activa el ocultamiento de superficies por z-buffer
	glClearColor(1.0, 1.0, 1.0, 0.0); // establece el color de fondo de la ventana

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
	case 'w': interfaz.posicion[2] += 0.1;
		break;
	case 'a': interfaz.posicion[0] += 0.1;
		break;
	case 's': interfaz.posicion[2] -= 0.1;
		break;
	case 'd': interfaz.posicion[0] -= 0.1;
		break;
	case 'q': interfaz.posicion[1] -= 0.1;
		break;
	case 'r': interfaz.posicion[1] += 0.1;
		break;
	case 'i': if (interfaz.pitch < 1)
		interfaz.pitch += 0.1;
		break;
	case 'k': if (interfaz.pitch > -0.5)
		interfaz.pitch -= 0.1;
		break;
	case 'j': interfaz.yaw -= 0.1;
		break;
	case 'l': interfaz.yaw += 0.1;
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
	case 'g': //Crea una figura de tipo monigote
		interfaz.escena.crearMuebles(0);
		break;
	case 'b':
		interfaz.escena.borrarMuebles();
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
		interfaz.posicion = interfaz.perspectivas[0][interfaz.tipoPerspectiva];
		interfaz.centro = interfaz.perspectivas[1][interfaz.tipoPerspectiva];
		interfaz.vector_up = interfaz.perspectivas[2][interfaz.tipoPerspectiva];
		break;
		////// Apartado D: incluir aquí la modificación de los grados de libertad del modelo pulsando las teclas correspondientes
	case 'm': interfaz.objeto_seleccionado++;
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
	interfaz.actualiza_camara();
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


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el z-buffer
	// se establece el viewport
	glViewport(0, 0, interfaz.get_ancho_ventana(), interfaz.get_alto_ventana());

	///// Apartado E: antes de aplicar las transformaciones de cámara y proyección hay que comprobar el modo para sólo visualizar o seleccionar:
	if (interfaz.modo == IGV_SELECCIONAR) {
		// Apartado E: Para que funcione habrá que dibujar la escena sin efectos, sin iluminación, sin texturas ...
		glDisable(GL_LIGHTING); // desactiva la iluminacion de la escena

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_CULL_FACE);

		// Apartado E: Reestablece los colores como no seleccionado

		// Apartado E: aplica la cámara

		// Apartado E: visualiza los BV cada uno de un color
		interfaz.escena.visualizar_colores();
		// Apartado E: Obtener el color del pixel seleccionado
		int y;
		y = interfaz.get_alto_ventana() - interfaz.cursorY;

		GLubyte pixel[3];
		glReadPixels(interfaz.cursorX, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
		// Apartado E: Comprobar el color del objeto que hay en el cursor mirando en la tabla de colores
		interfaz.escena.seleccion_color(pixel);
		// Apartado E: Asigna al objeto seleccionado el color amarillo

		// Apartado E: Cambiar a modo de visualización de la escena

		//glutSwapBuffers();
		// Apartado E: Habilitar de nuevo la iluminación
		glEnable(GL_LIGHTING);
	}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// establece los parámetros de la cámara y de la proyección
		interfaz.camara.aplicar();

		//visualiza la escena
		interfaz.escena.visualizar();

		// refresca la ventana
		glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo
	
}


void igvInterfaz::set_glutMouseFunc(GLint boton, GLint estado, GLint x, GLint y) {

	// Apartado E: comprobar que se ha pulsado el botón izquierdo 
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN) {
		if (!interfaz.boton_retenido) {
			// Apartado E: guardar que el boton se ha presionado o se ha soltado, si se ha pulsado hay que
			// pasar a modo IGV_SELECCIONAR
			interfaz.botonPulsado = GLUT_LEFT_BUTTON;
			interfaz.boton_retenido = true;
			// Apartado E: guardar el pixel pulsado
			interfaz.cursorX = x;
			interfaz.cursorY = y;
			interfaz.modo = IGV_SELECCIONAR;
		}
	}
	else if (boton == GLUT_RIGHT_BUTTON && estado == GLUT_DOWN) {
		if (!interfaz.boton_retenido) {
			interfaz.botonPulsado = GLUT_RIGHT_BUTTON;
			interfaz.cursorX = x;
			interfaz.cursorY = y;
			interfaz.boton_retenido = true;
		}
	}

	if (estado == GLUT_UP) {
		interfaz.boton_retenido = false;
		interfaz.cursorX = INT_MAX;
		interfaz.cursorY = INT_MAX;
		interfaz.modo = IGV_VISUALIZAR;
	}
	// Apartado E: renovar el contenido de la ventana de vision 

}

void igvInterfaz::set_glutMotionFunc(GLint x, GLint y) {

	// Apartado F: si el botón está retenido y hay algún objeto seleccionado,
	// comprobar el objeto seleccionado y la posición del ratón y actualizar
	// convenientemente el grado de libertad del objeto correspondiente 
	if (interfaz.boton_retenido) {
		if (interfaz.botonPulsado == GLUT_RIGHT_BUTTON) {
			float sumaX = interfaz.cursorX - x;
			float sumaY = interfaz.cursorY - y;
			if (interfaz.pitch > -0.5 && sumaY < 0) {
				interfaz.pitch += sumaY / 5000;
			}
			else {
				if (interfaz.pitch < 1 && sumaY > 0)
					interfaz.pitch += sumaY / 5000;
			}
			interfaz.yaw -= sumaX / 5000;
		}
		else {

		}
	}
	// Apartado F: guardar la nueva posición del ratón 


	interfaz.actualiza_camara();

	// Apartado F: renovar el contenido de la ventana de vision 
	glutPostRedisplay();

}

void igvInterfaz::set_glutIdleFunc() {
	///// Apartado G: incluir el código para animar el modelo de la manera más realista posible

}

void igvInterfaz::inicializa_callbacks() {
	glutKeyboardFunc(set_glutKeyboardFunc);
	glutReshapeFunc(set_glutReshapeFunc);
	glutDisplayFunc(set_glutDisplayFunc);
	glutSpecialFunc(set_glutSpecialFunc);

	glutMouseFunc(set_glutMouseFunc);
	glutMotionFunc(set_glutMotionFunc);
	glutIdleFunc(set_glutIdleFunc);
}
