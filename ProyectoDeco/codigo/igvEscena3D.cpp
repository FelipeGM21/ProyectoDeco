#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"
#include "igvMaterial.h"

// Metodos constructores 


igvEscena3D::igvEscena3D() {
	ejes = true;
	bv = false;

	/*Inicializacion de la matriz de ocupacion*/
	altura = 4;
	largo = 7;
	ancho = 7;
	matrizOcupacion = new int**[largo];
	for (int x = 0; x < largo; ++x) {
		matrizOcupacion[x] = new int*[altura];
		for (int y = 0; y < altura; ++y) {
			matrizOcupacion[x][y] = new int[ancho];
			for (int z = 0; z < ancho; ++z) {
				matrizOcupacion[x][y][z] = 0;
			}
		}
	}
	
	/*Inicializacion de las posiciones*/
	numeroFiguras = 10;
	figurasDisponibles = 10;
	figurasColocadas = 0;
	figuras = new int[figurasDisponibles];
	posiciones = new int*[figurasDisponibles];
	for (int i = 0; i < figurasDisponibles; i++) {
		figuras[i] = 0;
		posiciones[i] = new int[2];
		posiciones[i][0] = 0;
		posiciones[i][1] = 0;
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
	igvColor Kambiental(0.5, 0.3, 0.3, 1.0);
	igvColor Kdifuso(0.5, 0.1, 0.1, 1.0);
	igvColor Kespectacular(0.5, 0.1, 0.1, 1.0);
	double phong(120);
	igvMaterial rojo(Kambiental, Kdifuso, Kespectacular, phong);
	rojo.aplicar();
	glPushMatrix();

	//Posicionar en baldosa 0,0
	glTranslatef(0.5,1.55,0.5);

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

	rojo.aplicar();
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

	rojo.aplicar();
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

	rojo.aplicar();
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

	rojo.aplicar();
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


void igvEscena3D::pintar_quad(float div_x, float div_z) {
	float ini_x = 0.0;
	float ini_z = 0.0;
	float tam_x = 1.0;
	float tam_z = 1.0;

	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(ini_x, 0.0, ini_z);
	glVertex3f(ini_x, 0.0, ini_z + tam_z);
	glVertex3f(ini_x + tam_x, 0.0, ini_z + tam_z);
	glVertex3f(ini_x + tam_x, 0.0, ini_z);
	glEnd();
}

void igvEscena3D::crearSuelo(int tamx, int tamy) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco,kDifuso,kSpecular,1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);

	
	for (int x = 0; x < tamx; x++) {
		for (int z = 0; z < tamy; z++) {
			glPushMatrix();
			glTranslatef(x, 0, z);
			if ((x + z)%2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
}

void igvEscena3D::crearPared(int alto, int largo) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	
	for (int x = 0; x < largo; x++) {
		for (int z = 0; z < alto; z++) {
			glPushMatrix();
			glTranslatef(x, 0, z);
			if ((x + z) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
}


void igvEscena3D::seleccionar(int parte) {
	parteSeleccionada = parte;
}

void igvEscena3D::cambiarParte(int eje, int incremento) {
	if (parteSeleccionada >= 0 && parteSeleccionada < 9) {
		matrizRotaciones[parteSeleccionada][eje] += incremento;
		if (parteSeleccionada > 0 && matrizRotaciones[parteSeleccionada][eje] > 90) {
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

void igvEscena3D::pintarFiguraBV() {

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

void igvEscena3D::activar_BV() {
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

	igvPunto3D posicion(1.0, 1.0, 1.0);
	igvColor ambiental(0.5, 0.5, 0.5, 1.0);
	igvColor difuso(1.0, 1.0, 1.0, 1.0);
	igvColor espectacular(1.0, 1.0, 1.0, 1.0);
	double at0(1.0), at1(0.0), at2(0.0);
	igvFuenteLuz luzPuntual(GL_LIGHT0, posicion, ambiental, difuso, espectacular,
		at0, at1, at2);
	luzPuntual.aplicar();
	
	igvPunto3D posicionF(1.0, 3.0, 1.0);
	igvColor ambientalF(1, 1, 1, 1.0);
	igvColor difusoF(1.0, 1.0, 1.0, 1.0);
	igvColor espectacularF(1.0, 1.0, 1.0, 1.0);
	double at0F(1.0), at1F(0.0), at2F(0.0);
	igvPunto3D dirFoco(0.0, 0.0, 0.0);
	double anguloFoco = 80;
	double expoFoco = 1000;
	igvFuenteLuz luzFocal(GL_LIGHT1, posicionF, ambientalF, difusoF, espectacularF,
		at0F, at1F, at2F,dirFoco,anguloFoco,expoFoco);
	
///// Apartado B: aquí hay que añadir la visualización del árbol del modelo utilizando la pila de matrices de OpenGL
/////             se recomienda crear una método auxiliar que encapsule todo el código para la visualización
/////             del modelo, dejando aquí sólo la llamada a ese método, así como distintas funciones una para cada
/////			  parte del modelo. 
	crearSuelo(largo,ancho);
	glPushMatrix();
	figurasColocadas = 2;
	figuras[0] = 0;
	figuras[1] = 1;
	posiciones[0][0] = 1;
	posiciones[0][1] = 3;
	posiciones[1][0] = 2;
	posiciones[1][1] = 2;
	for (int i = 0; i < figurasColocadas; i++) {
		glPushMatrix();
		glTranslatef(posiciones[i][0], 0, posiciones[i][1]);
		switch (figuras[i]) {
		case 0:
			pintarFigura();
			break;
		case 1: glutSolidCube(1);
			break;
		}
		glPopMatrix();
	}
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	crearPared(altura,largo);
	glPopMatrix();
	glRotatef(-90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	crearPared(altura,ancho);
	glPopMatrix();
	//visualizar_BV();

	///// En el apartado E habrá que asignar un color de selección al objeto para indicar que la parte seleccionada se
	/////			visualice de un color distinto



	glPopMatrix(); // restaura la matriz de modelado
}


// Apartado E: Método/s para indicar el objeto seleccionado



