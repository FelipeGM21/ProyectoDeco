#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"

// Metodos constructores 


igvEscena3D::igvEscena3D() {
	ejes = true;
	bv = false;

	// Apartado D: inicializar los atributos para el control de los grados de libertad del modelo 
	for (int partes = 0; partes < 9; partes++) {
		for (int ejes = 0; ejes < 3; ejes++) {
			matrizRotaciones[partes][ejes] = 0;
		}
	}

	// Apartado E: inicializar el atributo/s que indica/n el objeto seleccionado para pintarlo de otro color
	parteSeleccionada = -1;
}

igvEscena3D::~igvEscena3D() {

}


// Metodos publicos 

void pintar_ejes(void) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	GLfloat verde[] = { 0,1,0,1.0 };
	GLfloat azul[] = { 0,0,1,1.0 };

	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	glBegin(GL_LINES);
	glVertex3f(1000, 0, 0);
	glVertex3f(-1000, 0, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, verde);
	glBegin(GL_LINES);
	glVertex3f(0, 1000, 0);
	glVertex3f(0, -1000, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 1000);
	glVertex3f(0, 0, -1000);
	glEnd();
}

void tube() {
	static GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, 0.25, 0.25, 1, 15, 15);
}

///// Apartado B: Métodos para visualizar cada parte del modelo

void igvEscena3D::cuerpo()
{
	glPushMatrix();
	//Brazo derecho
	glPushMatrix();
	glTranslatef(-0.25, 0.1, 0);
	glutSolidSphere(0.12, 60, 60);
	glPopMatrix();
	//Brazo izquierdo
	glPushMatrix();
	glTranslatef(0.25, 0.1, 0);
	glutSolidSphere(0.12, 60, 60);
	glPopMatrix();
	//Pierna derecha
	glPushMatrix();
	glTranslatef(-0.15, -0.25, 0);
	glutSolidSphere(0.12, 60, 60);
	glPopMatrix();
	//Piernza izquierda
	glPushMatrix();
	glTranslatef(0.15, -0.25, 0);
	glutSolidSphere(0.12, 60, 60);
	glPopMatrix();

	glutSolidCube(0.55);
	glPopMatrix();
}

void igvEscena3D::cabeza()
{
	glPushMatrix();
	glScalef(1.2, 1.2, 1.2);
	//Cuerno Izquierdo
	glColor3ub(200, 120, 7);
	glPushMatrix();
	glTranslatef(-0.2, 0.2, 0.2);
	glRotatef(60, 0, 0, 1);
	glScalef(0.1, 0.5, 0.1);
	glutSolidTetrahedron();
	glPopMatrix();
	//Cuerno Derecho
	glPushMatrix();
	glTranslatef(0.2, 0.2, 0.2);
	glRotatef(-60, 0, 0, 1);
	glScalef(0.1, 0.5, 0.1);
	glutSolidTetrahedron();
	glPopMatrix();
	//Esfera Cabeza
	glColor3ub(155, 155, 155);
	glutSolidSphere(0.3, 60, 60);
	glPopMatrix();
}

void igvEscena3D::brazo()
{
	glPushMatrix();
	glutSolidSphere(0.12, 60, 60);
	glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	tube();
	glPopMatrix();
}

void igvEscena3D::antebrazo()
{
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	tube();
	glPopMatrix();
}

void igvEscena3D::muslo()
{
	glPushMatrix();
	glutSolidSphere(0.12, 60, 60);
	glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	tube();
	glPopMatrix();
}

void igvEscena3D::pierna()
{
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0, 0, -0.08);
	glScaled(1, 1.3, 2.2);
	glutSolidCube(0.2);
	glPopMatrix();
	glRotatef(90, 0, 1, 0);
	glScalef(0.5, 0.5, 0.7);
	tube();
	glPopMatrix();
}

void igvEscena3D::pintarFigura() {
	
	//Comenzamos a dibujar la figura
	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	glPushMatrix();

	//Rotacion figura completa
	glRotatef(matrizRotaciones[0][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[0][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[0][ejez], 0, 0, 1);
	glPushMatrix();
	if (parteSeleccionada == 0)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	cuerpo();
	glPopMatrix();
	//Cabeza
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	cabeza();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	////Brazo izquierdo
	glPushMatrix();
	glTranslatef(0.25, 0.1, 0);
	glRotatef(matrizRotaciones[1][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[1][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[1][ejez], 0, 0, 1);
	glTranslatef(0.5, 0, 0);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	if (parteSeleccionada == 1)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	brazo();
	glPopMatrix();
	glRotatef(matrizRotaciones[2][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[2][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[2][ejez], 0, 0, 1);
	glPushMatrix();
	if (parteSeleccionada == 2)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	antebrazo();
	glPopMatrix();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	////Brazo derecho
	glPushMatrix();
	glTranslatef(-0.25, 0.1, 0);
	glRotatef(matrizRotaciones[3][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[3][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[3][ejez], 0, 0, 1);
	glTranslatef(-0.5, 0, 0);
	glPushMatrix();
	//glRotatef(180, 0, 1, 0);
	if (parteSeleccionada == 3)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	brazo();
	glPopMatrix();
	glRotatef(matrizRotaciones[4][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[4][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[4][ejez], 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	if (parteSeleccionada == 4)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	antebrazo();
	glPopMatrix();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	//Pierna izquierda
	glPushMatrix();
	glTranslatef(0.15, -0.25, 0);
	glRotatef(matrizRotaciones[5][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[5][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[5][ejez], 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(-0.5, 0, 0);
	glPushMatrix();
	if (parteSeleccionada == 5)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	muslo();
	glPopMatrix();
	glRotatef(matrizRotaciones[6][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[6][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[6][ejez], 0, 0, 1);
	glTranslatef(-0.7, 0, 0);
	glPushMatrix();
	if (parteSeleccionada == 6)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pierna();
	glPopMatrix();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	//Pierna derecha
	glPushMatrix();
	glTranslatef(-0.15, -0.25, 0);
	glRotatef(matrizRotaciones[7][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[7][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[7][ejez], 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(-0.5, 0, 0);
	glPushMatrix();
	if (parteSeleccionada == 7)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	muslo();
	glPopMatrix();
	glRotatef(matrizRotaciones[8][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[8][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[8][ejez], 0, 0, 1);
	glTranslatef(-0.7, 0, 0);
	glPushMatrix();
	if (parteSeleccionada == 8)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pierna();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}


void igvEscena3D::seleccionar(int parte) {
	parteSeleccionada = parte;
}

void igvEscena3D::cambiarParte(int eje, int incremento) {
	if (parteSeleccionada >= 0 && parteSeleccionada < 9) {
		matrizRotaciones[parteSeleccionada][eje] += incremento;
		if (parteSeleccionada > 0 && matrizRotaciones[parteSeleccionada][eje] > 90 ) {
			matrizRotaciones[parteSeleccionada][eje] = 90;
		}
		else {
			if (parteSeleccionada > 0 && matrizRotaciones[parteSeleccionada][eje] < -90)
				matrizRotaciones[parteSeleccionada][eje] = -90;
		}
	}
}

////// Apartado C: Métodos para visualizar los volúmenes envolventes (BV) 

void igvEscena3D::pintar_BV(float escalaX, float escalaY, float escalaZ, GLubyte *color) {
	///// Apartado C: Es importante no usar materiales, sino colores de OpenGL
	glPushMatrix();
	glColor3ubv(color);
	///// Apartado C: Incluir el codigo para pintar un Volumen Envolvente unidad, escalado y del color determinado
	glScalef(escalaX, escalaY, escalaZ);
	glutSolidCube(0.5);
	glPopMatrix();


}

void igvEscena3D::pintarFiguraBV(){
	
	GLubyte color[3];
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;

	//Comenzamos a dibujar la figura
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	//Rotacion figura completa
	glRotatef(matrizRotaciones[0][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[0][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[0][ejez], 0, 0, 1);
	glPushMatrix();
	glTranslatef(0, 0.6, 0);
	if (parteSeleccionada == 0)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.5, 1.5, 1.5, color);
	//Cuernos
	glTranslatef(0, 0.25, 0.2);
	//Cuerno izq
	glPushMatrix();
	glTranslatef(-0.45, 0, 0);
	pintar_BV(0.6, 0.6, 0.6, color);
	glPopMatrix();
	//Cuerno der
	glPushMatrix();
	glTranslatef(0.45, 0, 0);
	pintar_BV(0.6, 0.6, 0.6, color);
	glPopMatrix();
	glPopMatrix();
	//Cabeza BV
	
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	pintar_BV(0.8, 0.8, 0.8, color);
	glPopMatrix();
	color[0] = 200;

	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	////Brazo izquierdo
	glPushMatrix();
	glRotatef(matrizRotaciones[1][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[1][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[1][ejez], 0, 0, 1);
	glTranslatef(0.75, 0.1, 0);
	glPushMatrix();
	glTranslatef(-0.2, 0, 0);
	if (parteSeleccionada == 1)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.3, 0.7, 0.7, color);
	glPopMatrix();
	glRotatef(matrizRotaciones[2][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[2][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[2][ejez], 0, 0, 1);
	glPushMatrix();
	glTranslatef(0.35, 0, 0);
	color[1] = 100;
	if (parteSeleccionada == 2)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.4, 0.7, 0.7, color);
	glPopMatrix();
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	////Brazo derecho
	color[0] = 0;
	color[1] = 200;
	glPushMatrix();
	glTranslatef(-0.25, 0.1, 0);
	glRotatef(matrizRotaciones[3][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[3][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[3][ejez], 0, 0, 1);
	glTranslatef(-0.5, 0, 0);
	glPushMatrix();
	glTranslatef(0.2, 0, 0);
	if (parteSeleccionada == 3)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.3, 0.7, 0.7, color);
	glPopMatrix();
	glRotatef(matrizRotaciones[4][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[4][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[4][ejez], 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	glTranslatef(0.35, 0, 0);
	color[0] = 100;
	if (parteSeleccionada == 4)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.4, 0.7, 0.7, color);
	glPopMatrix();
	glPopMatrix();

	color[0] = 0;
	color[2] = 200;
	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	//Pierna izquierda
	glPushMatrix();
	glTranslatef(0.15, -0.25, 0);
	glRotatef(matrizRotaciones[5][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[5][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[5][ejez], 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(-0.5, 0, 0);
	glPushMatrix();
	glTranslatef(0.2, 0, 0);
	if (parteSeleccionada == 5)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.3, 0.5, 0.5, color);
	glPopMatrix();
	glRotatef(matrizRotaciones[6][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[6][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[6][ejez], 0, 0, 1);
	glTranslatef(-0.7, 0, 0);
	glPushMatrix();
	color[1] = 100;
	glTranslatef(0.3, 0, 0);
	if (parteSeleccionada == 6)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.5, 0.5, 0.5, color);
	glTranslatef(-0.3, 0, -0.1);
	pintar_BV(0.5, 0.55, 1, color);
	glPopMatrix();
	glPopMatrix();

	color[0] = 200;
	color[1] = 0;
	glMaterialfv(GL_FRONT, GL_EMISSION, gris);
	//Pierna derecha
	glPushMatrix();
	glTranslatef(-0.15, -0.25, 0);
	glRotatef(matrizRotaciones[7][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[7][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[7][ejez], 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glTranslatef(-0.5, 0, 0);
	glPushMatrix();
	glTranslatef(0.2, 0, 0);
	if (parteSeleccionada == 7)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.3, 0.5, 0.5, color);
	glPopMatrix();
	glRotatef(matrizRotaciones[8][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[8][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[8][ejez], 0, 0, 1);
	glTranslatef(-0.7, 0, 0);
	glPushMatrix();
	color[1] = 100;
	glTranslatef(0.3, 0, 0);
	if (parteSeleccionada == 8)
		glMaterialfv(GL_FRONT, GL_EMISSION, amarillo);
	pintar_BV(1.5, 0.5, 0.5, color);
	glTranslatef(-0.3, 0, -0.1);
	pintar_BV(0.5, 0.55, 1, color);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

void igvEscena3D::visualizar_BV() {
	///// Apartado C: aquí hay que añadir la visualización del árbol de Bounding Volume del modelo utilizando la pila de matrices de OpenGL
	/////             se recomienda crear una método auxiliar que encapsule todo el código para la visualización
	/////             del modelo, dejando aquí sólo la llamada a ese método. 
	if (bv) {
		pintarFiguraBV();
	}
	///// Apartado E: Incluir el color de selección adecuado para cada parte



}

void igvEscena3D::activar_BV(){
	bv = !bv;
}



void igvEscena3D::visualizar() {
	// crear luces
	GLfloat luz0[4] = { 5.0,5.0,5.0,1 }; // luz puntual  
	glLightfv(GL_LIGHT0, GL_POSITION, luz0); // la luz se coloca aquí si permanece fija y no se mueve con la escena
	glEnable(GL_LIGHT0);

	// crear el modelo
	glPushMatrix(); // guarda la matriz de modelado

	// se pintan los ejes
	if (ejes) pintar_ejes();

	//glLightfv(GL_LIGHT0,GL_POSITION,luz0); // la luz se coloca aquí si se mueve junto con la escena

///// Apartado B: aquí hay que añadir la visualización del árbol del modelo utilizando la pila de matrices de OpenGL
/////             se recomienda crear una método auxiliar que encapsule todo el código para la visualización
/////             del modelo, dejando aquí sólo la llamada a ese método, así como distintas funciones una para cada
/////			  parte del modelo. 
	
	pintarFigura();	
	visualizar_BV();

	///// En el apartado E habrá que asignar un color de selección al objeto para indicar que la parte seleccionada se
	/////			visualice de un color distinto



	glPopMatrix(); // restaura la matriz de modelado
}


// Apartado E: Método/s para indicar el objeto seleccionado



