#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"
#include "igvMaterial.h"
#include <utility>
#include <iostream>
// Metodos constructores 

//Constructor por defecto
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
				matrizOcupacion[x][y][z] = -1;
			}
		}
	}

	/*Inicializacion de las posiciones*/
	numeroFiguras = 10;
	figurasDisponibles = 10;
	figurasColocadas = 1;
	vector<int> p;
	p.push_back(1);
	p.push_back(3);
	arrayPosiciones.push_back(p);
	arrayFiguras.push_back(0);
	figuras[0] = 0;
	posiciones[0][0] = 1;
	posiciones[0][1] = 3;

	x_seleccionada = -1;
	y_seleccionada = -1;
	z_seleccionada = -1;
	estado_selec = baldosa::null;

	// Apartado E: inicializar el atributo/s que indica/n el objeto seleccionado para pintarlo de otro color
	parteSeleccionada = -1;
}

//Destructor por defecto
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

//Crea un cilindro
void tube() {
	static GLUquadric* quad = gluNewQuadric();
	gluCylinder(quad, 0.25, 0.25, 1, 15, 15);
}

//Metodos para pintar la figura
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
	glColor3ub(155, 155, 155);
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
//Pinta la figura
void igvEscena3D::pintarFigura(int numFigura) {

	//Comenzamos a dibujar la figura
	float color[3];
	int x = posiciones[numFigura][0];
	int z = posiciones[numFigura][1];
	matrizOcupacion[x][0][z] = numFigura;
	matrizOcupacion[x][1][z] = numFigura;
	matrizOcupacion[x + 1][1][z] = numFigura;
	matrizOcupacion[x - 1][1][z] = numFigura;

	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		color[0] = 1;
		color[1] = 1;
		color[2] = 0;
	}
	else {
		color[0] = 0.5;
		color[1] = 0.3;
		color[2] = 0.3;
	}
	igvColor Kambiental(color[0], color[1], color[2], 1.0);
	igvColor Kdifuso(0.5, 0.1, 0.1, 1.0);
	igvColor Kespectacular(0.5, 0.1, 0.1, 1.0);
	double phong(120);

	igvMaterial materialBasico(Kambiental, Kdifuso, Kespectacular, phong);

	glPushMatrix();
	materialBasico.aplicar();
	//Posicionar en baldosa 0,0
	glTranslatef(0.5, 1.55, 0.5);

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

	materialBasico.aplicar();
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

	materialBasico.aplicar();
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

	materialBasico.aplicar();
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

	materialBasico.aplicar();
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
//Pinta la figura preparada para seleccion de color
void igvEscena3D::pintarFiguraC(GLubyte color[3], int numFigura) {

	//Comenzamos a dibujar la figura

	glPushMatrix();
	glColor3ubv(color);
	baldosa asulejo(baldosa::mueble, numFigura, numFigura, numFigura);
	int sColor = get_number(color);
	mapa_baldosas[sColor] = asulejo;
	//Posicionar en baldosa 0,0
	glTranslatef(0.5, 1.55, 0.5);

	//Rotacion figura completa
	glRotatef(matrizRotaciones[0][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[0][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[0][ejez], 0, 0, 1);
	glPushMatrix();
	cuerpo();
	glPopMatrix();
	//Cabeza
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	cabeza();
	glPopMatrix();

	glColor3ubv(color);
	////Brazo izquierdo
	glPushMatrix();
	glTranslatef(0.25, 0.1, 0);
	glRotatef(matrizRotaciones[1][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[1][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[1][ejez], 0, 0, 1);
	glTranslatef(0.5, 0, 0);
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	brazo();
	glPopMatrix();
	glRotatef(matrizRotaciones[2][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[2][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[2][ejez], 0, 0, 1);
	glPushMatrix();
	antebrazo();
	glPopMatrix();
	glPopMatrix();

	glColor3ubv(color);
	////Brazo derecho
	glPushMatrix();
	glTranslatef(-0.25, 0.1, 0);
	glRotatef(matrizRotaciones[3][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[3][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[3][ejez], 0, 0, 1);
	glTranslatef(-0.5, 0, 0);
	glPushMatrix();
	//glRotatef(180, 0, 1, 0);
	brazo();
	glPopMatrix();
	glRotatef(matrizRotaciones[4][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[4][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[4][ejez], 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	antebrazo();
	glPopMatrix();
	glPopMatrix();

	glColor3ubv(color);
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
	muslo();
	glPopMatrix();
	glRotatef(matrizRotaciones[6][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[6][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[6][ejez], 0, 0, 1);
	glTranslatef(-0.7, 0, 0);
	glPushMatrix();
	pierna();
	glPopMatrix();
	glPopMatrix();

	glColor3ubv(color);
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
	muslo();
	glPopMatrix();
	glRotatef(matrizRotaciones[8][ejex], 1, 0, 0);
	glRotatef(matrizRotaciones[8][ejey], 0, 1, 0);
	glRotatef(matrizRotaciones[8][ejez], 0, 0, 1);
	glTranslatef(-0.7, 0, 0);
	glPushMatrix();
	pierna();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}

//Pinta un quad
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

//No sirven
void igvEscena3D::crearSuelo(int tamx, int tamz, int y) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);

	glPushMatrix();
	glTranslatef(0, y, 0);

	for (int x = 0; x < tamx; x++) {
		for (int z = 0; z < tamz; z++) {
			glPushMatrix();
			conteoColor++;
			figuras[conteoColor] = 11;
			posiciones[conteoColor][0] = x;
			posiciones[conteoColor][1] = y;
			posiciones[conteoColor][2] = z;
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
void igvEscena3D::crearParedX(int alto, int largo, int z) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);

	glPushMatrix();
	glTranslatef(largo, 0, z);
	glRotatef(-90, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	for (int x = 0; x < largo; x++) {
		for (int y = 0; y < alto; y++) {
			glPushMatrix();
			posiciones[conteoColor][0] = x;
			posiciones[conteoColor][1] = y;
			posiciones[conteoColor][2] = z;

			glTranslatef(x, 0, y);
			if ((x + y) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}
void igvEscena3D::crearParedZ(int alto, int profundo, int x) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);

	glPushMatrix();
	glTranslatef(x, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);

	for (int z = 0; z < profundo; z++) {
		for (int y = 0; y < alto; y++) {
			glPushMatrix();
			posiciones[conteoColor][0] = x;
			posiciones[conteoColor][1] = y;
			posiciones[conteoColor][2] = z;
			glTranslatef(z, 0, y);
			if ((z + y) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Crea suelo
void igvEscena3D::crearSueloB(int tamx, int tamz, int y, baldosa::posicion estado) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor seleccion = igvColor(1, 1, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	igvMaterial seleccionado = igvMaterial(seleccion, kDifuso, kSpecular, 1);

	glPushMatrix();
	glTranslatef(0, y, 0);
	int f = 10;

	for (int x = 0; x < tamx; x++) {
		for (int z = 0; z < tamz; z++) {
			glPushMatrix();
			GLubyte color[3] = { 255 - f * x  ,0,255 - f * z };
			baldosa asulejo(estado, x, y, z);
			int sColor = get_number(color);
			//printf("Suelo, baldosa %d x,%d z, color %d \n",x,z,sColor);
			mapa_baldosas[sColor] = asulejo;
			glTranslatef(x, 0, z);
			if ((x + z) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			if (estado_selec == baldosa::suelo) {
				if (x_seleccionada == x && y_seleccionada == y && z_seleccionada == z) {
					seleccionado.aplicar();
				}
			}
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Crea pared 1
void igvEscena3D::crearParedXB(int alto, int largo, int z, baldosa::posicion estado) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor seleccion = igvColor(1, 1, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	igvMaterial seleccionado = igvMaterial(seleccion, kDifuso, kSpecular, 1);

	glPushMatrix();
	glTranslatef(largo, 0, z);
	glRotatef(-90, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	int f = 10;

	for (int x = 0; x < largo; x++) {
		for (int y = 0; y < alto; y++) {
			glPushMatrix();
			GLubyte color[3] = { 255 - f * x ,255 - f * y,0 };
			glColor3ubv(color);
			baldosa asulejo(estado, x, y, z);
			int sColor = get_number(color);
			//printf("ParedFondo, baldosa %d x,%d y, color %d \n", x, y, sColor);
			mapa_baldosas[sColor] = asulejo;
			glTranslatef(x, 0, y);
			if ((x + y) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			if (estado_selec == baldosa::pared1) {
				if (x_seleccionada == x && y_seleccionada == y && z_seleccionada == z) {
					seleccionado.aplicar();
				}
			}
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Crea pared 2
void igvEscena3D::crearParedZB(int alto, int profundo, int x, baldosa::posicion estado) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor seleccion = igvColor(1, 1, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	igvMaterial seleccionado = igvMaterial(seleccion, kDifuso, kSpecular, 1);

	glPushMatrix();
	glTranslatef(x, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	int f = 10;

	for (int z = 0; z < profundo; z++) {
		for (int y = 0; y < alto; y++) {
			glPushMatrix();
			GLubyte color[3] = { 0,255 - f * y ,255 - f * z };
			baldosa asulejo(estado, x, y, z);
			int sColor = get_number(color);
			//printf("ParedLat, baldosa %d y,%d z, color %d \n", y, z, sColor);
			mapa_baldosas[sColor] = asulejo;
			glTranslatef(z, 0, y);
			if ((y + z) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			if (estado_selec == baldosa::pared2) {
				if (x_seleccionada == x && y_seleccionada == y && z_seleccionada == z) {
					seleccionado.aplicar();
				}
			}
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Metodo para crear en la escena un mueble en la baldosa seleccionada
void igvEscena3D::crearMuebles(int tecla) {
	if (estado_selec == baldosa::suelo) {
		switch (tecla) {
		case 0:
			if (x_seleccionada != 0 && x_seleccionada != ancho - 1) {
				if (
					matrizOcupacion[x_seleccionada][0][z_seleccionada] == -1 && matrizOcupacion[x_seleccionada + 1][1][z_seleccionada] == -1 &&
					matrizOcupacion[x_seleccionada][1][z_seleccionada] == -1 && matrizOcupacion[x_seleccionada - 1][1][z_seleccionada] == -1
					) {
					arrayFiguras.push_back(tecla);
					posiciones[figurasColocadas][0] = x_seleccionada;
					posiciones[figurasColocadas][1] = z_seleccionada;
					figurasColocadas++;
					vector<int> p;
					p.push_back(x_seleccionada);
					p.push_back(z_seleccionada);
					arrayPosiciones.push_back(p);
				}
			}
		}
	}
	else if (estado_selec == baldosa::pared1 || estado_selec == baldosa::pared2) {

	}
}

//Metodo para borrar muebles, borra el mueble seleccionado
void igvEscena3D::borrarMuebles() {
	if (estado_selec != baldosa::suelo && estado_selec != baldosa::pared1 && estado_selec != baldosa::pared2 && estado_selec != baldosa::null) {
		arrayFiguras.erase(arrayFiguras.begin() + x_seleccionada);
		arrayPosiciones.erase(arrayPosiciones.begin() + x_seleccionada);
		matrizOcupacion[posiciones[x_seleccionada][0]][0][posiciones[x_seleccionada][1]] = -1;
		matrizOcupacion[posiciones[x_seleccionada][0]][1][posiciones[x_seleccionada][1]] = -1;
		matrizOcupacion[posiciones[x_seleccionada][0] + 1][1][posiciones[x_seleccionada][1]] = -1;
		matrizOcupacion[posiciones[x_seleccionada][0] - 1][1][posiciones[x_seleccionada][1]] = -1;
		figurasColocadas--;
		estado_selec = baldosa::null;
	}
}

//Crea suelo preparado para la seleccion de color
void igvEscena3D::crearSueloBC(int tamx, int tamz, int y, baldosa::posicion estado) {
	glPushMatrix();
	glTranslatef(0, y, 0);
	int f = 10;

	for (int x = 0; x < tamx; x++) {
		for (int z = 0; z < tamz; z++) {
			glPushMatrix();
			GLubyte color[3] = { 255 - f * x  ,0,255 - f * z };
			glColor3ubv(color);
			baldosa asulejo(estado, x, y, z);
			int sColor = get_number(color);
			//printf("Suelo, baldosa %d x,%d z, color %d \n", x, z, sColor);
			mapa_baldosas[sColor] = asulejo;
			glTranslatef(x, 0, z);
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Crea pared1 preparado para la seleccion de color
void igvEscena3D::crearParedXBC(int alto, int largo, int z, baldosa::posicion estado) {
	glPushMatrix();
	glTranslatef(largo, 0, z);
	glRotatef(-90, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	int f = 10;

	for (int x = 0; x < largo; x++) {
		for (int y = 0; y < alto; y++) {
			glPushMatrix();
			GLubyte color[3] = { 255 - f * x ,255 - f * y,0 };
			glColor3ubv(color);
			baldosa asulejo(estado, x, y, z);
			int sColor = get_number(color);
			//printf("ParedFondo, baldosa %d x,%d y, color %d \n", x, y, sColor);
			mapa_baldosas[sColor] = asulejo;
			glTranslatef(x, 0, y);
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Crea pared2 preparado para la seleccion de color
void igvEscena3D::crearParedZBC(int alto, int profundo, int x, baldosa::posicion estado) {
	glPushMatrix();
	glTranslatef(x, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	int f = 10;

	for (int z = 0; z < profundo; z++) {
		for (int y = 0; y < alto; y++) {
			glPushMatrix();
			GLubyte color[3] = { 0,255 - f * y,255 - f * z };
			glColor3ubv(color);
			int sColor = get_number(color);
			//printf("ParedLat, baldosa %d y,%d z, color %d \n", y, z, sColor);
			baldosa asulejo(estado, x, y, z);
			mapa_baldosas[sColor] = asulejo;
			glTranslatef(z, 0, y);
			pintar_quad(1, 1);
			glPopMatrix();
		}
	}
	glPopMatrix();
}

//Metodo que dado un color selecciona el objeto
void igvEscena3D::seleccion_color(GLubyte color[3]) {
	int sColor = get_number(color);
	baldosa bal = mapa_baldosas[sColor];
	if (estado_selec == baldosa::mueble) {
		if (bal.estado == baldosa::suelo) {
			printf("Posicion X:%d \nPosicion Z:%d\n\n", bal.posX, bal.posZ);
			switch (arrayFiguras[x_seleccionada]) {
			case 0:
				if (bal.posX != 0 && bal.posX != ancho - 1) {
					if (
						(matrizOcupacion[bal.posX][0][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][0][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX + 1][1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX + 1][1][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX][1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][1][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX - 1][1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX - 1][1][bal.posZ] == -1)
						) {

						matrizOcupacion[arrayPosiciones[x_seleccionada][0]][0][arrayPosiciones[x_seleccionada][1]] = -1;
						matrizOcupacion[arrayPosiciones[x_seleccionada][0]][1][arrayPosiciones[x_seleccionada][1] + 1] = -1;
						matrizOcupacion[arrayPosiciones[x_seleccionada][0]][1][arrayPosiciones[x_seleccionada][1]] = -1;
						matrizOcupacion[arrayPosiciones[x_seleccionada][0]][1][arrayPosiciones[x_seleccionada][1] - 1] = -1;

						arrayPosiciones[x_seleccionada][0] = bal.posX;
						arrayPosiciones[x_seleccionada][1] = bal.posZ;
					}
				}
			}

		}
	}
	x_seleccionada = bal.posX;
	y_seleccionada = bal.posY;
	z_seleccionada = bal.posZ;
	estado_selec = bal.get_estado();
}

//Metodo para seleccionar la parte de la figura
void igvEscena3D::seleccionar(int parte) {
	parteSeleccionada = parte;
}
//Metodo para cambiar la parte de la figura a mover
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

//Metodo para visualizar la escena
void igvEscena3D::visualizar() {
	// crear luces
	// la luz se coloca aquí si permanece fija y no se mueve con la escena

	//Crear luz ambiental
	glEnable(GL_LIGHT0);
	GLfloat luz[4] = { 0.2, 0.2, 0.2, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, luz);

	//Comienzo de creacion de escena
	glPushMatrix();
	// se pintan los ejes
	if (ejes) pintar_ejes();

	//Crear luz puntual
	glEnable(GL_LIGHT1);
	igvPunto3D posicion(3.0, 1.0, 3.0);
	igvColor ambiental(0.5, 0.5, 0.5, 1.0);
	igvColor difuso(1.0, 1.0, 1.0, 1.0);
	igvColor espectacular(1.0, 1.0, 1.0, 1.0);
	double at0(0.2), at1(0.2), at2(0.2);
	igvFuenteLuz luzPuntual(GL_LIGHT1, posicion, ambiental, difuso, espectacular,
		at0, at1, at2);
	luzPuntual.aplicar();

	//Crear luz focal
	glEnable(GL_LIGHT2);
	igvPunto3D posicionF(1.0, 4.0, 1.0);
	igvColor ambientalF(0.0, 0.0, 0.0, 1.0);
	igvColor difusoF(0.0, 0.0, 0.3, 0.7);
	igvColor espectacularF(0.0, 0.0, 0.3, 0.7);
	double at0F(0.3), at1F(0.3), at2F(0.3);
	igvPunto3D dirFoco(0.0, -4.0, 0.0);
	double anguloFoco = 50;
	double expoFoco = 1;
	igvFuenteLuz luzFocal(GL_LIGHT2, posicionF, ambientalF, difusoF, espectacularF,
		at0F, at1F, at2F, dirFoco, anguloFoco, expoFoco);
	luzFocal.aplicar();

	//Crea la habitacion
	glPushMatrix();
	crearSueloB(largo, ancho, 0, baldosa::suelo);
	crearParedXB(altura, largo, 0, baldosa::pared1);
	crearParedZB(altura, ancho, 0, baldosa::pared2);
	glPopMatrix();

	//Coloca todas las figuras de la escena en sus posiciones
	for (int i = 0; i < figurasColocadas; i++) {
		glPushMatrix();
		glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
		//Se coloca la figura predeterminada
		switch (arrayFiguras[i]) {
		case 0:
			pintarFigura(i);
			break;
		case 1: glutSolidCube(1);
			break;
		}
		glPopMatrix();
	}

	glPopMatrix(); // restaura la matriz de modelado
}

//Metodo para visualizar la escena, preparada para seleccion de color
void igvEscena3D::visualizar_colores() {

	mapa_baldosas.clear();

	// crear el modelo
	glPushMatrix(); // guarda la matriz de modelado

	//Apagamos las luces
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHT2);

	//Creamos suelo y paredes
	crearSueloBC(largo, ancho, 0, baldosa::suelo);
	crearParedXBC(altura, largo, 0, baldosa::pared1);
	crearParedZBC(altura, ancho, 0, baldosa::pared2);

	//Comenzamos a colorear las figuras
	glPushMatrix();
	GLubyte color[3];

	for (int i = 0; i < figurasColocadas; i++) {
		glPushMatrix();
		color[0] = i;
		color[1] = i;
		color[2] = i;
		glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
		switch (figuras[i]) {
		case 0:
			pintarFiguraC(color, i);
			break;
		case 1: glColor3ubv(color);
			glutSolidCube(1);
			break;
		}
		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix(); // restaura la matriz de modelado
}
// Apartado E: Método/s para indicar el objeto seleccionado



