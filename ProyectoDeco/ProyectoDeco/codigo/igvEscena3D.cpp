#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"
#include <utility>
#include <iostream>
// Metodos constructores 

//Constructor por defecto
igvEscena3D::igvEscena3D() :suelo("./suelo.bmp"), pared("./pared.bmp"), madera("./mesa.bmp"), marco("./marco.bmp"), paisaje("./paisaje.bmp"),
suelo_seleccion("./suelo_seleccion.bmp"), pared_seleccion("./pared_seleccion.bmp"), madera_seleccion("./mesa_seleccionada.bmp"),
marco_seleccion("./marco_seleccionado.bmp"), paisaje_seleccion("./paisaje_seleccionado.bmp"), fondo_parte1("./fondo1.bmp"),
fondo_parte2("./fondo2.bmp"), fondo_parte1_seleccion("./fondo1_seleccion.bmp"), fondo_parte2_seleccion("./fondo2_seleccion.bmp"),
hojas("./hojas.bmp"), hojas_seleccion("./hojas_seleccion.bmp"), palo("./palo.bmp"), palo_seleccion("./palo_seleccion.bmp"),
tierra_tex("./tierra.bmp"), tierra_seleccion("./tierra_seleccion.bmp"), maceta_tex("./maceta.bmp"), maceta_seleccion("./maceta_seleccion.bmp") {
	ejes = true;
	bv = false;

	/*Inicializacion de la matriz de ocupacion*/
	altura = 4;
	largo = 7;
	ancho = 7;
	matrizOcupacion = new int**[largo + 1];
	for (int x = 0; x < largo + 1; ++x) {
		matrizOcupacion[x] = new int*[altura + 1];
		for (int y = 0; y < altura + 1; ++y) {
			matrizOcupacion[x][y] = new int[ancho + 1];
			for (int z = 0; z < ancho + 1; ++z) {
				matrizOcupacion[x][y][z] = -1;
			}
		}
	}

	/*Inicializacion de las posiciones*/
	numeroFiguras = 10;
	figurasDisponibles = 10;
	figurasColocadas = 0;

	x_seleccionada = -1;
	y_seleccionada = -1;
	z_seleccionada = -1;
	estado_selec = baldosa::null;

	const char *archivo = "./suelo.bmp";
	suelo = igvTextura(archivo);
	archivo = "./pared.bmp";
	pared = igvTextura(archivo);


	// Apartado E: inicializar el atributo/s que indica/n el objeto seleccionado para pintarlo de otro color
	parteSeleccionada = -1;
}

//Destructor por defecto
igvEscena3D::~igvEscena3D() {

}

static void drawBox(GLfloat size, GLenum type, const char*archivo)
{
	static GLfloat n[6][3] =
	{
	  {-1.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0},
	  {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0},
	  {0.0, 0.0, 1.0},
	  {0.0, 0.0, -1.0}
	};
	static GLint faces[6][4] =
	{
	  {0, 1, 2, 3},
	  {3, 2, 6, 7},
	  {7, 6, 5, 4},
	  {4, 5, 1, 0},
	  {5, 6, 2, 1},
	  {7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	igvTextura textura(archivo);

	textura.aplicar();

	for (i = 5; i >= 0; i--) {
		glBegin(type);
		glNormal3fv(&n[i][0]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(&v[faces[i][3]][0]);
		glEnd();

	}
	textura.~igvTextura();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}
static void drawBox(GLfloat size, GLenum type, igvTextura textura)
{
	static GLfloat n[6][3] =
	{
	  {-1.0, 0.0, 0.0},
	  {0.0, 1.0, 0.0},
	  {1.0, 0.0, 0.0},
	  {0.0, -1.0, 0.0},
	  {0.0, 0.0, 1.0},
	  {0.0, 0.0, -1.0}
	};
	static GLint faces[6][4] =
	{
	  {0, 1, 2, 3},
	  {3, 2, 6, 7},
	  {7, 6, 5, 4},
	  {4, 5, 1, 0},
	  {5, 6, 2, 1},
	  {7, 4, 0, 3}
	};
	GLfloat v[8][3];
	GLint i;

	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

	glPushMatrix();
	textura.aplicar();
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvColor igvMarron = igvColor(1.04f, 0.59f, 0.16f);
	igvMaterial marron = igvMaterial(igvMarron, kDifuso, kSpecular, 1);
	marron.aplicar();
	for (i = 5; i >= 0; i--) {
		glBegin(type);
		glNormal3fv(&n[i][0]);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(&v[faces[i][0]][0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(&v[faces[i][1]][0]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(&v[faces[i][2]][0]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(&v[faces[i][3]][0]);
		glEnd();

	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
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

void glutSolidCube(GLfloat size, const char*archivo)
{
	drawBox(size, GL_QUADS, archivo);
}
void glutSolidCube(GLfloat size, igvTextura archivo)
{
	drawBox(size, GL_QUADS, archivo);
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
void igvEscena3D::pintarFigura(int numFigura, int rota) {

	//Comenzamos a dibujar la figura
	float color[3];

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
	glRotatef(rota, 0, 1, 0);
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
void igvEscena3D::pintarFiguraC(GLubyte color[3], int numFigura, int rota) {

	//Comenzamos a dibujar la figura

	glPushMatrix();
	glColor3ubv(color);
	baldosa asulejo(baldosa::mueble, numFigura, numFigura, numFigura);
	int sColor = get_number(color);
	mapa_baldosas[sColor] = asulejo;
	//Posicionar en baldosa 0,0
	glTranslatef(0.5, 1.55, 0.5);

	//Rotacion figura completa
	glRotatef(rota, 0, 1, 0);
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

void igvEscena3D::silla(int numFigura) {

	glPushMatrix();
	igvColor Kambiental(0.15, 0, 0, 1.0);
	igvColor Kdifuso(0.5, 0, 0, 1.0);
	igvColor Kespectacular(0.5, 0.0, 0.0, 1.0);
	double phong(120);
	igvMaterial material(Kambiental, Kdifuso, Kespectacular, phong);
	material.aplicar();
	const char *archivo = "./mesa.bmp";

	//pata izquierda trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//pata derecha trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//pata derecha delantera
	glPushMatrix();
	glTranslatef(0.855, 0.28, 0.125);
	glScalef(0.5, 2.1, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//pata izquierda delantera
	glPushMatrix();
	glTranslatef(0.855, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//asiento
	glPushMatrix();
	glTranslatef(0.5, 0.6, 0.5);
	glScalef(3.4, 0.5, 3.5);
	glRotatef(90, 1, 0, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//
	glPushMatrix();
	glTranslatef(0.15, 0.98, 0.88);
	glScalef(0.5, 2.5, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0.98, 0.125);
	glScalef(0.5, 2.5, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 1, 0.5);
	glScalef(0.5, 0.5, 3.5);
	glRotatef(90, 0, 1, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 1.3, 0.5);
	glScalef(0.5, 0.5, 3.5);
	glRotatef(90, 0, 1, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	glPopMatrix();

}
void igvEscena3D::sillaC(GLubyte color[3], int numFigura) {
	glPushMatrix();

	glColor3ubv(color);
	baldosa asulejo(baldosa::mueble, numFigura, numFigura, numFigura);
	int sColor = get_number(color);
	mapa_baldosas[sColor] = asulejo;

	//pata izquierda trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//pata derecha trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//pata derecha delantera
	glPushMatrix();
	glTranslatef(0.855, 0.28, 0.125);
	glScalef(0.5, 2.1, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//pata izquierda delantera
	glPushMatrix();
	glTranslatef(0.855, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//asiento
	glPushMatrix();
	glTranslatef(0.5, 0.6, 0.5);
	glScalef(3.4, 0.5, 3.5);
	glRotatef(90, 1, 0, 0);
	glutSolidCube(0.25);
	glPopMatrix();

	//
	glPushMatrix();
	glTranslatef(0.15, 0.98, 0.88);
	glScalef(0.5, 2.5, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0.98, 0.125);
	glScalef(0.5, 2.5, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 1, 0.5);
	glScalef(0.5, 0.5, 3.5);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(0.25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 1.3, 0.5);
	glScalef(0.5, 0.5, 3.5);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(0.25);
	glPopMatrix();

	glPopMatrix();
}
void igvEscena3D::mesa(int numFigura) {
	glPushMatrix();
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvColor igvMarron = igvColor(1.04f, 0.59f, 0.16f);
	igvMaterial marron = igvMaterial(igvMarron, kDifuso, kSpecular, 1);
	marron.aplicar();
	const char *archivo = "./mesa.bmp";

	//pata izquierda trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//pata derecha trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//pata derecha delantera
	glPushMatrix();
	glTranslatef(1.855, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//pata izquierda delantera
	glPushMatrix();
	glTranslatef(1.855, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();

	//superficie
	glPushMatrix();
	glTranslatef(1, 0.6, 0.5);
	glScalef(7.4, 0.5, 3.5);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.25, madera_seleccion);
	}
	else {
		glutSolidCube(0.25, madera);
	}
	glPopMatrix();
	glPopMatrix();

}
void igvEscena3D::mesaC(GLubyte color[3], int numFigura) {
	glPushMatrix();

	glColor3ubv(color);
	baldosa asulejo(baldosa::mueble, numFigura, numFigura, numFigura);
	int sColor = get_number(color);
	mapa_baldosas[sColor] = asulejo;

	//pata izquierda trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//pata derecha trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//pata derecha delantera
	glPushMatrix();
	glTranslatef(1.855, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//pata izquierda delantera
	glPushMatrix();
	glTranslatef(1.855, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	//superficie
	glPushMatrix();
	glTranslatef(1, 0.6, 0.5);
	glScalef(7.4, 0.5, 3.5);
	glutSolidCube(0.25);
	glPopMatrix();
	glPopMatrix();

}
void igvEscena3D::pintar_quad(float div_x, float div_z, const char*archivo) {
	float ini_x = 0.0;
	float ini_z = 0.0;
	float tam_x = 1.0;
	float tam_z = 1.0;

	igvTextura textura(archivo);

	textura.aplicar();
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(ini_x, 0.0, ini_z);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(ini_x, 0.0, ini_z + tam_z);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(ini_x + tam_x, 0.0, ini_z + tam_z);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(ini_x + tam_x, 0.0, ini_z);
	glEnd();
	textura.~igvTextura();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void igvEscena3D::pintar_quad(float div_x, float div_z, igvTextura textura) {
	float ini_x = 0.0;
	float ini_z = 0.0;
	float tam_x = 1.0;
	float tam_z = 1.0;
	glEnable(GL_TEXTURE_2D);
	textura.aplicar();
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(ini_x, 0.0, ini_z);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(ini_x, 0.0, ini_z + tam_z);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(ini_x + tam_x, 0.0, ini_z + tam_z);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(ini_x + tam_x, 0.0, ini_z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void igvEscena3D::crearSuelo(int tamx, int tamy) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0, 0, 0);
	igvColor kSpecular = igvColor(0, 0, 0);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	const char *archivo = "./suelo.bmp";

	for (int x = 0; x < tamx; x++) {
		for (int z = 0; z < tamy; z++) {
			glPushMatrix();
			glTranslatef(x, 0, z);
			if ((x + z) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			pintar_quad(1, 1, archivo);
			glPopMatrix();
		}
	}
}

void igvEscena3D::crearPared(int alto, int largo) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0, 0, 0);
	igvColor kSpecular = igvColor(0, 0, 0);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	const char *archivo = "./pared.bmp";

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
			pintar_quad(1, 1, archivo);
			glPopMatrix();
		}
	}
}

void igvEscena3D::pintar_triangulo3D() {

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 1.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 1.0);

	glTexCoord2f(1.0f, 0.1f); glVertex3f(1.0, 0.0, 1.0);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0, 1.0, 0.0);

	glTexCoord2f(1.0f, 0.1f);	glVertex3f(1.0, 0.0, 0.0);

	glEnd();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0, 1.0, 1.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 0.0);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0, 0.0, 1.0);



	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 1.0);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 1.0);


	glVertex3f(0.0, 0.0, 0.0);

	glVertex3f(1.0, 1.0, 0.0);

	glVertex3f(1.0, 1.0, 1.0);

	glVertex3f(0.0, 0.0, 1.0);

	glEnd();
	glPopMatrix();
}
void igvEscena3D::pintar_triangulo3D(const char* archivo) {
	GLfloat azul[] = { 0,0,1,1.0 };

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);

	igvTextura textura(archivo);
	textura.aplicar();

	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 1.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 1.0);

	glTexCoord2f(1.0f, 0.1f); glVertex3f(1.0, 0.0, 1.0);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0, 1.0, 0.0);

	glTexCoord2f(1.0f, 0.1f);	glVertex3f(1.0, 0.0, 0.0);

	glEnd();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0, 1.0, 1.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 0.0);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0, 0.0, 1.0);



	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 1.0);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 1.0);


	glVertex3f(0.0, 0.0, 0.0);

	glVertex3f(1.0, 1.0, 0.0);

	glVertex3f(1.0, 1.0, 1.0);

	glVertex3f(0.0, 0.0, 1.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}
void igvEscena3D::pintar_triangulo3D(igvTextura archivo) {
	GLfloat azul[] = { 0,0,1,1.0 };
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, azul);

	archivo.aplicar();

	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 1.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 1.0);

	glTexCoord2f(1.0f, 0.1f); glVertex3f(1.0, 0.0, 1.0);

	glTexCoord2f(0.0f, 0.0f);	glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 0.0f);	glVertex3f(1.0, 1.0, 0.0);

	glTexCoord2f(1.0f, 0.1f);	glVertex3f(1.0, 0.0, 0.0);

	glEnd();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0, 1.0, 1.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 1.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 0.0);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0, 0.0, 1.0);



	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0, 0.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0, 0.0, 1.0);

	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 1.0);


	glVertex3f(0.0, 0.0, 0.0);

	glVertex3f(1.0, 1.0, 0.0);

	glVertex3f(1.0, 1.0, 1.0);

	glVertex3f(0.0, 0.0, 1.0);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void igvEscena3D::parte_cuadro() {

	//const char *archivo = "./silla.bmp";

	glPushMatrix();
	glTranslatef(0.075, 0.42, 0.275);
	glScalef(0.6, 2.2, 0.59);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.2);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.69, 0.35);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D();
	glPopMatrix();
}
void igvEscena3D::parte_cuadro(const char *archivo) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);

	//const char *archivo = "./silla.bmp";

	glPushMatrix();
	glTranslatef(0.075, 0.42, 0.275);
	glScalef(0.6, 2.2, 0.59);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.2);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D(archivo);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.69, 0.35);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D(archivo);
	glPopMatrix();
}
void igvEscena3D::parte_cuadro(igvTextura textura) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);

	//const char *archivo = "./silla.bmp";

	glPushMatrix();
	glTranslatef(0.075, 0.42, 0.275);
	glScalef(0.6, 2.2, 0.59);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, textura);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.2);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D(textura);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.69, 0.35);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D(textura);
	glPopMatrix();
}
void igvEscena3D::parte_cuadro2(igvTextura textura) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);


	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(0.075, 0.42, 0.275);
	glScalef(0.6, 2.2, 0.59);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, textura);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.69, 0.35);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D(textura);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glPushMatrix();
	glTranslatef(0.075, 0.42, 0.275);
	glScalef(0.6, 2.2, 0.59);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, textura);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.2);
	glScalef(0.15, 0.15, 0.15);
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	pintar_triangulo3D(textura);
	glPopMatrix();
	glPopMatrix();
}
void igvEscena3D::cuadro(int numFigura) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);

	const char *archivo = "./marco.bmp";
	const char *archivo2 = "./paisaje.bmp";

	glPushMatrix();
	glTranslatef(0, -1, -1);
	//izquierda
	glPushMatrix();
	glTranslatef(0.15, 1.1, 2.13);
	glRotatef(180, 0, 1, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro(marco_seleccion);
	}
	else {
		parte_cuadro(marco);
	}
	glPopMatrix();

	//derecha
	glPushMatrix();
	glTranslatef(0.0, 1.1, 0.89);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro(marco_seleccion);
	}
	else {
		parte_cuadro(marco);
	}
	glPopMatrix();

	//arriba
	glPushMatrix();
	glTranslatef(0.0, 2.13, 1.09);
	glRotatef(90, 1, 0, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro(marco_seleccion);
	}
	else {
		parte_cuadro(marco);
	}
	glPopMatrix();

	//abajo
	glPushMatrix();
	glTranslatef(0.15, 0.9, 1.09);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro(marco_seleccion);
	}
	else {
		parte_cuadro(marco);
	}
	glPopMatrix();

	//fondo

	//glRotatef(90, 0, 0, 1);
	//glScalef(0.4, 0.4, 0.4);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		paisaje_seleccion.aplicar();
	}
	else {
		paisaje.aplicar();
	}
	glTranslatef(0.05, 0.3, 0.3);
	glScalef(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

}
void igvEscena3D::cuadroC(GLubyte color[3], int numFigura) {
	glPushMatrix();

	glColor3ubv(color);
	baldosa asulejo(baldosa::mueble, numFigura, numFigura, numFigura);
	int sColor = get_number(color);
	mapa_baldosas[sColor] = asulejo;

	glTranslatef(0, -1, -1);
	//izquierda
	glPushMatrix();
	glTranslatef(0.15, 1.1, 2.13);
	glRotatef(180, 0, 1, 0);
	parte_cuadro();
	glPopMatrix();

	//derecha
	glPushMatrix();
	glTranslatef(0.0, 1.1, 0.89);
	parte_cuadro();
	glPopMatrix();

	//arriba
	glPushMatrix();
	glTranslatef(0.0, 2.13, 1.09);
	glRotatef(90, 1, 0, 0);
	parte_cuadro();
	glPopMatrix();

	//abajo
	glPushMatrix();
	glTranslatef(0.15, 0.9, 1.09);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	parte_cuadro();
	glPopMatrix();

	//fondo

	//glRotatef(90, 0, 0, 1);
	//glScalef(0.4, 0.4, 0.4);
	glColor3ubv(color);
	glTranslatef(0.05, 0.3, 0.3);
	glScalef(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
}
void igvEscena3D::cuadroV(int numFigura) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);

	//izquierda
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.15, 0.85, 2.13);
	glRotatef(180, 0, 1, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro2(marco_seleccion);
	}
	else {

		parte_cuadro2(marco);
	}
	glPopMatrix();

	//derecha
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.0, 0.85, 0.89);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro2(marco_seleccion);
	}
	else {
		parte_cuadro2(marco);
	}
	glPopMatrix();

	//arriba
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.0, 2.89, 1.09);
	glRotatef(90, 1, 0, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro(marco_seleccion);
	}
	else {
		parte_cuadro(marco);
	}
	glPopMatrix();

	//abajo
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.15, 1.15, 1.09);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		parte_cuadro(marco_seleccion);
	}
	else {
		parte_cuadro(marco);
	}
	glPopMatrix();

	//fondo
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		fondo_parte2_seleccion.aplicar();
	}
	else {
		fondo_parte2.aplicar();
	}
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.05, 0.8, 0.6);
	glScalef(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		fondo_parte1_seleccion.aplicar();
	}
	else {
		fondo_parte1.aplicar();
	}
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.05, 1.4, 0.6);
	glScalef(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void igvEscena3D::cuadroVC(GLubyte color[3], int numFigura) {
	glPushMatrix();
	glColor3ubv(color);

	baldosa asulejo(baldosa::mueble, numFigura, numFigura, numFigura);
	int sColor = get_number(color);
	mapa_baldosas[sColor] = asulejo;

	//izquierda
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.15, 0.85, 2.13);
	glRotatef(180, 0, 1, 0);
	glPopMatrix();

	//derecha
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.0, 0.85, 0.89);
	glPopMatrix();

	//arriba
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.0, 2.89, 1.09);
	glRotatef(90, 1, 0, 0);
	glPopMatrix();

	//abajo
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.15, 1.15, 1.09);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glPopMatrix();

	//fondo
	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.05, 0.8, 0.6);
	glScalef(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -1, -1);
	glTranslatef(0.05, 1.4, 0.6);
	glScalef(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}
void igvEscena3D::planta(int numFigura) {
	GLfloat azul[] = { 0,0,1,1.0 };

	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	glPushMatrix();
	glTranslatef(0.5, 0.9, 0.5);
	glScalef(0.25, 3.2, 0.25);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(0.5, palo_seleccion);
	}
	else {
		glutSolidCube(0.5, palo);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.6, 0.5);
	glScalef(0.4, 0.25, 0.4);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(1, hojas_seleccion);
	}
	else {

		glutSolidCube(1, hojas);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.3, 0.5);
	glScalef(0.5, 0.25, 0.5);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(1, hojas_seleccion);
	}
	else {

		glutSolidCube(1, hojas);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1, 0.5);
	glScalef(0.6, 0.25, 0.6);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(1, hojas_seleccion);
	}
	else {

		glutSolidCube(1, hojas);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.7, 0.5);
	glScalef(0.7, 0.25, 0.7);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		glutSolidCube(1, hojas_seleccion);
	}
	else {

		glutSolidCube(1, hojas);
	}
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.3);
	glScalef(0.5, 0.5, 0.5);
	if (estado_selec == baldosa::mueble && x_seleccionada == numFigura) {
		maceta(maceta_seleccion);
		tierra(tierra_seleccion);
	}
	else {
		maceta(maceta_tex);
		tierra(tierra_tex);
	}

	glPopMatrix();
	glPopMatrix();
}
void igvEscena3D::plantaC(GLubyte color[3], int numFigura) {

	glPushMatrix();
	glColor3ubv(color);

	baldosa asulejo(baldosa::mueble, numFigura, numFigura, numFigura);
	int sColor = get_number(color);
	mapa_baldosas[sColor] = asulejo;

	glPushMatrix();
	glTranslatef(0.5, 0.9, 0.5);
	glScalef(0.25, 3.2, 0.25);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.6, 0.5);
	glScalef(0.4, 0.25, 0.4);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.3, 0.5);
	glScalef(0.5, 0.25, 0.5);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1, 0.5);
	glScalef(0.6, 0.25, 0.6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.7, 0.5);
	glScalef(0.7, 0.25, 0.7);
	glutSolidCube(1);
	glPopMatrix();

	glTranslatef(0.3, 0.0, 0.3);
	glScalef(0.5, 0.5, 0.5);
	maceta();
	tierra();

	glPopMatrix();
}
void igvEscena3D::tierra(igvTextura textura) {
	glPushMatrix();
	glBegin(GL_QUADS);
	textura.aplicar();
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.4, 0.7);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.4, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.4, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.4, 0.7);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void igvEscena3D::tierra() {
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.4, 0.7);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.4, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.4, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.4, 0.7);
	glEnd();
	glPopMatrix();

}
void igvEscena3D::maceta(igvTextura textura) {
	glPushMatrix();
	GLfloat verde[] = { 0,1,0,1.0 };
	GLfloat rojo[] = { 1,0,0,1.0 };
	const char *tierra_base = "./tierra.bmp";
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_EMISSION, verde);

	textura.aplicar();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 0.7);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.0, 0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.0, 0.7);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.8, 0.5, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 0.5, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0.0, 0.2);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 0.5, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.5, 0.8);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.7);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0.0, 0.2);


	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.8, 0.5, 0.8);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.8, 0.5, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.0, 0.7);


	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.5, 0.8);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.8, 0.5, 0.8);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.0, 0.7);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.7);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glEnd();
	glPopMatrix();
}
void igvEscena3D::maceta() {
	glPushMatrix();

	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 0.7);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.0, 0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.0, 0.7);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.8, 0.5, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 0.5, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.2, 0.0, 0.2);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.0, 0.0);

	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 0.5, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.5, 0.8);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.7);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.2, 0.0, 0.2);


	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.8, 0.5, 0.8);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.8, 0.5, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.0, 0.7);


	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.5, 0.8);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.8, 0.5, 0.8);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.0, 0.7);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 0.7);

	glEnd();
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

//Crea suelo
void igvEscena3D::crearSueloB(int tamx, int tamz, int y, baldosa::posicion estado) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor seleccion = igvColor(1, 1, 0);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0.1, 0.1, 0.1);
	igvColor kSpecular = igvColor(0.1, 0.1, 0.1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	igvMaterial blanco = igvMaterial(kBlanco, kDifuso, kSpecular, 1);
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
			//const char *archivo = "./suelo.bmp";
			if ((x + z) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			if (estado_selec == baldosa::suelo) {
				if (x_seleccionada == x && y_seleccionada == y && z_seleccionada == z) {
					seleccionado.aplicar();
					pintar_quad(1, 1, suelo_seleccion);
				}
				else {
					pintar_quad(1, 1, suelo);
				}
			}
			else {
				pintar_quad(1, 1, suelo);
			}
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
			const char *archivo = "./pared.bmp";
			if ((x + y) % 2 == 0) {
				blanco.aplicar();
			}
			else {
				negro.aplicar();
			}
			if (estado_selec == baldosa::pared1) {
				if (x_seleccionada == x && y_seleccionada == y && z_seleccionada == z) {
					seleccionado.aplicar();
					pintar_quad(1, 1, pared_seleccion);
				}
				else {
					pintar_quad(1, 1, pared);
				}
			}
			else {
				pintar_quad(1, 1, pared);
			}

			//pintar_quad(1, 1);

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
			const char *archivo = "./pared.bmp";
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
					pintar_quad(1, 1, pared_seleccion);
				}
				else {
					pintar_quad(1, 1, pared);
				}
			}
			else {
				pintar_quad(1, 1, pared);
			}
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
					figurasColocadas++;
					vector<int> p;
					p.push_back(x_seleccionada);
					p.push_back(z_seleccionada);
					arrayPosiciones.push_back(p);
				}
			}
			break;
		case 1:
			if (matrizOcupacion[x_seleccionada][0][z_seleccionada] == -1 && matrizOcupacion[x_seleccionada][1][z_seleccionada] == -1) {
				arrayFiguras.push_back(4);
				figurasColocadas++;
				vector<int> p;
				p.push_back(x_seleccionada);
				p.push_back(z_seleccionada);
				arrayPosiciones.push_back(p);
			}
			break;
		case 2:
			if (z_seleccionada != ancho - 1) {
				if (
					matrizOcupacion[x_seleccionada][0][z_seleccionada] == -1 && matrizOcupacion[x_seleccionada + 1][0][z_seleccionada] == -1
					) {
					arrayFiguras.push_back(5);
					figurasColocadas++;
					vector<int> p;
					p.push_back(x_seleccionada);
					p.push_back(z_seleccionada);
					arrayPosiciones.push_back(p);
				}
			}
			break;
		case 5:
			if (matrizOcupacion[x_seleccionada][0][z_seleccionada] == -1 && matrizOcupacion[x_seleccionada][1][z_seleccionada] == -1) {
				arrayFiguras.push_back(8);
				figurasColocadas++;
				vector<int> p;
				p.push_back(x_seleccionada);
				p.push_back(z_seleccionada);
				arrayPosiciones.push_back(p);
			}
			break;
		}
	}
	else if (estado_selec == baldosa::pared1) {
		if (tecla == 3) {
			if (matrizOcupacion[x_seleccionada][y_seleccionada][z_seleccionada] == -1) {
				arrayFiguras.push_back(16);
				figurasColocadas++;
				vector<int> p;
				p.push_back(x_seleccionada);
				p.push_back(y_seleccionada);
				arrayPosiciones.push_back(p);
			}
		}
		else {
			if (tecla == 4) {
				if (y_seleccionada < altura - 1) {
					if (matrizOcupacion[x_seleccionada][y_seleccionada][z_seleccionada] == -1 &&
						matrizOcupacion[x_seleccionada][y_seleccionada + 1][z_seleccionada] == -1) {
						arrayFiguras.push_back(17);
						figurasColocadas++;
						vector<int> p;
						p.push_back(x_seleccionada);
						p.push_back(y_seleccionada);
						arrayPosiciones.push_back(p);
					}
				}
			}
		}
	}
	else if (estado_selec == baldosa::pared2) {
		if (tecla == 3) {
			if (
				matrizOcupacion[x_seleccionada][y_seleccionada][z_seleccionada] == -1) {
				arrayFiguras.push_back(6);
				figurasColocadas++;
				vector<int> p;
				p.push_back(z_seleccionada);
				p.push_back(y_seleccionada);
				arrayPosiciones.push_back(p);
			}
		}
		else {
			if (tecla == 4) {
				if (y_seleccionada < altura - 1) {
					if (matrizOcupacion[x_seleccionada][y_seleccionada][z_seleccionada] == -1 &&
						matrizOcupacion[x_seleccionada][y_seleccionada + 1][z_seleccionada] == -1) {
						arrayFiguras.push_back(7);
						figurasColocadas++;
						vector<int> p;
						p.push_back(z_seleccionada);
						p.push_back(y_seleccionada);
						arrayPosiciones.push_back(p);
					}
				}
			}
		}
	}
	actualizarOcupacion();
}

void igvEscena3D::rotarFigura() {
	int pos1;
	int pos2;
	if (estado_selec != baldosa::suelo && estado_selec != baldosa::pared1 && estado_selec != baldosa::pared2 && estado_selec != baldosa::null) {
		pos1 = arrayPosiciones[x_seleccionada][0];
		pos2 = arrayPosiciones[x_seleccionada][1];
		switch (arrayFiguras[x_seleccionada]) {
		case 0:
			if (pos2 != 0 && pos2 != altura - 1) {
				if (
					(matrizOcupacion[pos1][0][pos2] == x_seleccionada || matrizOcupacion[pos1][0][pos2] == -1) &&
					(matrizOcupacion[pos1][1][pos2 + 1] == x_seleccionada || matrizOcupacion[pos1][1][pos2 + 1] == -1) &&
					(matrizOcupacion[pos1][1][pos2] == x_seleccionada || matrizOcupacion[pos1][1][pos2] == -1) &&
					(matrizOcupacion[pos1][1][pos2 - 1] == x_seleccionada || matrizOcupacion[pos1][1][pos2 - 1] == -1)
					) {
					arrayFiguras[x_seleccionada] = 1;
				}
			}
			break;
		case 1:
			if (pos1 != 0 && pos1 != ancho - 1) {
				if (
					(matrizOcupacion[pos1][0][pos2] == x_seleccionada || matrizOcupacion[pos1][0][pos2] == -1) &&
					(matrizOcupacion[pos1 + 1][1][pos2] == x_seleccionada || matrizOcupacion[pos1 + 1][1][pos2] == -1) &&
					(matrizOcupacion[pos1][1][pos2] == x_seleccionada || matrizOcupacion[pos1][1][pos2] == -1) &&
					(matrizOcupacion[pos1 - 1][1][pos2] == x_seleccionada || matrizOcupacion[pos1 - 1][1][pos2] == -1)
					) {
					arrayFiguras[x_seleccionada] = 2;
				}
			}
			arrayFiguras[x_seleccionada] = 2;
			break;
		case 2:
			if (arrayPosiciones[x_seleccionada][1] != 0 && arrayPosiciones[x_seleccionada][1] != altura - 1) {
				if (
					(matrizOcupacion[pos1][0][pos2] == x_seleccionada || matrizOcupacion[pos1][0][pos2] == -1) &&
					(matrizOcupacion[pos1][1][pos2 + 1] == x_seleccionada || matrizOcupacion[pos1][1][pos2 + 1] == -1) &&
					(matrizOcupacion[pos1][1][pos2] == x_seleccionada || matrizOcupacion[pos1][1][pos2] == -1) &&
					(matrizOcupacion[pos1][1][pos2 - 1] == x_seleccionada || matrizOcupacion[pos1][1][pos2 - 1] == -1)
					) {
					arrayFiguras[x_seleccionada] = 3;
				}
			}
			break;
		case 3:
			if (pos1 != 0 && pos1 != ancho - 1) {
				if (
					(matrizOcupacion[pos1][0][pos2] == x_seleccionada || matrizOcupacion[pos1][0][pos2] == -1) &&
					(matrizOcupacion[pos1 + 1][1][pos2] == x_seleccionada || matrizOcupacion[pos1 + 1][1][pos2] == -1) &&
					(matrizOcupacion[pos1][1][pos2] == x_seleccionada || matrizOcupacion[pos1][1][pos2] == -1) &&
					(matrizOcupacion[pos1 - 1][1][pos2] == x_seleccionada || matrizOcupacion[pos1 - 1][1][pos2] == -1)
					) {
					arrayFiguras[x_seleccionada] = 0;
				}
			}
			break;
		case 4:
			arrayFiguras[x_seleccionada] = 14;
			break;
		case 14:
			arrayFiguras[x_seleccionada] = 24;
			break;
		case 24:
			arrayFiguras[x_seleccionada] = 34;
			break;
		case 34:
			arrayFiguras[x_seleccionada] = 4;
			break;
		case 5:
			if (pos1 != ancho - 1) {
				if ((matrizOcupacion[pos1][0][pos2] == x_seleccionada || matrizOcupacion[pos1][0][pos2] == -1) &&
					(matrizOcupacion[pos1 + 1][1][pos2] == x_seleccionada || matrizOcupacion[pos1 + 1][1][pos2] == -1)) {
					arrayFiguras[x_seleccionada] = 15;
				}
			}
			break;
		case 15:
			if (pos1 != 0 && pos1 != largo - 1) {
				if ((matrizOcupacion[pos1][0][pos2] == x_seleccionada || matrizOcupacion[pos1][0][pos2] == -1) &&
					(matrizOcupacion[pos1][1][pos2 + 1] == x_seleccionada || matrizOcupacion[pos1][1][pos2 + 1] == -1)) {
					arrayFiguras[x_seleccionada] = 5;
				}
			}
			break;
		}
	}
}
//Metodo para borrar muebles, borra el mueble seleccionado
void igvEscena3D::borrarMuebles() {
	if (estado_selec != baldosa::suelo && estado_selec != baldosa::pared1 && estado_selec != baldosa::pared2 && estado_selec != baldosa::null) {
		arrayFiguras.erase(arrayFiguras.begin() + x_seleccionada);
		arrayPosiciones.erase(arrayPosiciones.begin() + x_seleccionada);
		figurasColocadas--;
		estado_selec = baldosa::null;
		actualizarOcupacion();
	}

}
void igvEscena3D::actualizarOcupacion() {
	for (int x = 0; x < largo; ++x) {
		for (int y = 0; y < altura; ++y) {
			for (int z = 0; z < ancho; ++z) {
				matrizOcupacion[x][y][z] = -1;
			}
		}
	}
	for (int i = 0; i < arrayFiguras.size(); i++) {
		int x = arrayPosiciones[i][0];
		int z = arrayPosiciones[i][1];
		switch (arrayFiguras[i]) {
		case 0:
		case 2: //Figura en sentido x
			matrizOcupacion[x][0][z] = i;
			matrizOcupacion[x][1][z] = i;
			matrizOcupacion[x + 1][1][z] = i;
			matrizOcupacion[x - 1][1][z] = i;
			break;
		case 1:
		case 3: //figura en sentido z
			matrizOcupacion[x][0][z] = i;
			matrizOcupacion[x][1][z] = i;
			matrizOcupacion[x][1][z + 1] = i;
			matrizOcupacion[x][1][z - 1] = i;
			break;
		case 4: //Silla
		case 14:
		case 24:
		case 34:
			matrizOcupacion[x][0][z] = i;
			matrizOcupacion[x][1][z] = i;
			break;
		case 5: //Mesa
			matrizOcupacion[x][0][z] = i;
			matrizOcupacion[x + 1][0][z] = i;
			break;
		case 15:
			matrizOcupacion[x][0][z] = i;
			matrizOcupacion[x][0][z + 1] = i;
			break;
		case 6: //Cuadro
			matrizOcupacion[0][z][x] = i;
			break;
		case 16:
			matrizOcupacion[x][z][0] = i;
			break;
		case 7:
			matrizOcupacion[0][z][x] = i;
			matrizOcupacion[0][z + 1][x] = i;
			break;
		case 17:
			matrizOcupacion[x][z][0] = i;
			matrizOcupacion[x][z + 1][0] = i;
			break;
		case 8:
			matrizOcupacion[x][0][z] = i;
			matrizOcupacion[x][1][z] = i;
			break;
		}
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
	actualizarOcupacion();
	baldosa bal = mapa_baldosas[sColor];
	if (estado_selec == baldosa::mueble) {
		if (bal.estado == baldosa::suelo) {
			printf("Posicion X:%d \nPosicion Z:%d\n\n", bal.posX, bal.posZ);
			switch (arrayFiguras[x_seleccionada]) {
			case 0:
			case 2:
				if (bal.posX != 0 && bal.posX != ancho - 1) {
					if (
						(matrizOcupacion[bal.posX][0][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][0][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX + 1][1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX + 1][1][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX][1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][1][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX - 1][1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX - 1][1][bal.posZ] == -1)
						) {
						arrayPosiciones[x_seleccionada][0] = bal.posX;
						arrayPosiciones[x_seleccionada][1] = bal.posZ;
					}
				}
				break;
			case 1:
			case 3:
				if (bal.posZ != 0 && bal.posZ != largo - 1) {
					if (
						(matrizOcupacion[bal.posX][0][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][0][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX][1][bal.posZ + 1] == x_seleccionada || matrizOcupacion[bal.posX][1][bal.posZ + 1] == -1) &&
						(matrizOcupacion[bal.posX][1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][1][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX][1][bal.posZ - 1] == x_seleccionada || matrizOcupacion[bal.posX][1][bal.posZ - 1] == -1)
						) {
						arrayPosiciones[x_seleccionada][0] = bal.posX;
						arrayPosiciones[x_seleccionada][1] = bal.posZ;
					}
				}
				break;
			case 4: //Silla
				if (matrizOcupacion[bal.posX][0][bal.posZ] == -1 && matrizOcupacion[bal.posX][1][bal.posZ] == -1) {
					arrayPosiciones[x_seleccionada][0] = bal.posX;
					arrayPosiciones[x_seleccionada][1] = bal.posZ;
				}
				break;
			case 5: //Mesa
				if (bal.posX != 0 && bal.posX != ancho - 1) {
					if (
						(matrizOcupacion[bal.posX][0][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][0][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX + 1][0][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX + 1][0][bal.posZ] == -1)
						) {
						arrayPosiciones[x_seleccionada][0] = bal.posX;
						arrayPosiciones[x_seleccionada][1] = bal.posZ;
					}
				}
				break;
			case 15:
				if (bal.posZ != 0 && bal.posZ != largo - 1) {
					if (
						(matrizOcupacion[bal.posX][0][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][0][bal.posZ] == -1) &&
						(matrizOcupacion[bal.posX][0][bal.posZ + 1] == x_seleccionada || matrizOcupacion[bal.posX][0][bal.posZ + 1] == -1)
						) {
						arrayPosiciones[x_seleccionada][0] = bal.posX;
						arrayPosiciones[x_seleccionada][1] = bal.posZ;
					}
				}
				break;
			case 8:
				if (matrizOcupacion[bal.posX][0][bal.posZ] == -1 && matrizOcupacion[bal.posX][1][bal.posZ] == -1) {
					arrayPosiciones[x_seleccionada][0] = bal.posX;
					arrayPosiciones[x_seleccionada][1] = bal.posZ;
				}
				break;
			}
		}
		else {
			if (bal.estado == baldosa::pared1) {
				if (arrayFiguras[x_seleccionada] == 6 || arrayFiguras[x_seleccionada] == 16) {
					if (
						(matrizOcupacion[bal.posX][bal.posY][0] == x_seleccionada || matrizOcupacion[bal.posX][bal.posY][0] == -1)
						) {
						arrayFiguras[x_seleccionada] = 16;
						arrayPosiciones[x_seleccionada][0] = bal.posX;
						arrayPosiciones[x_seleccionada][1] = bal.posY;
					}
				}
				else {
					if (arrayFiguras[x_seleccionada] == 7 || arrayFiguras[x_seleccionada] == 17) {
						if (
							(matrizOcupacion[bal.posX][bal.posY][0] == x_seleccionada || matrizOcupacion[bal.posX][bal.posY][0] == -1) &&
							(matrizOcupacion[bal.posX][bal.posY + 1][0] == x_seleccionada || matrizOcupacion[bal.posX][bal.posY + 1][0] == -1)
							) {
							arrayFiguras[x_seleccionada] = 17;
							arrayPosiciones[x_seleccionada][0] = bal.posX;
							arrayPosiciones[x_seleccionada][1] = bal.posY;
						}
					}
				}
			}
			if (bal.estado == baldosa::pared2) {
				if (arrayFiguras[x_seleccionada] == 6 || arrayFiguras[x_seleccionada] == 16) {
					if (matrizOcupacion[0][bal.posY][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][bal.posY][bal.posZ] == -1) {
						arrayFiguras[x_seleccionada] = 6;
						arrayPosiciones[x_seleccionada][0] = bal.posZ;
						arrayPosiciones[x_seleccionada][1] = bal.posY;
					}
				}
				else {
					if (arrayFiguras[x_seleccionada] == 7 || arrayFiguras[x_seleccionada] == 17) {
						if (
							(matrizOcupacion[0][bal.posY][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][bal.posY][bal.posZ] == -1) &&
							(matrizOcupacion[0][bal.posY + 1][bal.posZ] == x_seleccionada || matrizOcupacion[bal.posX][bal.posY + 1][bal.posZ] == -1)
							) {
							arrayFiguras[x_seleccionada] = 7;
							arrayPosiciones[x_seleccionada][0] = bal.posZ;
							arrayPosiciones[x_seleccionada][1] = bal.posY;
						}
					}
				}
			}
		}
		actualizarOcupacion();
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

		//Se coloca la figura predeterminada
		switch (arrayFiguras[i]) {
		case 0: //Figura
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFigura(i, 0);
			break;
		case 1: //Figura 90º
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFigura(i, 90);
			break;
		case 2: //Figura 180º
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFigura(i, 180);
			break;
		case 3: //Figura 270º
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFigura(i, 270);
			break;
		case 4: //Silla
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			silla(i);
			break;
		case 14:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(0.5, 0, 0.5);
			glRotatef(90, 0, 1, 0);
			glTranslatef(-0.5, 0, -0.5);
			silla(i);
			break;
		case 24:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(0.5, 0, 0.5);
			glRotatef(180, 0, 1, 0);
			glTranslatef(-0.5, 0, -0.5);
			silla(i);
			break;
		case 34:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(0.5, 0, 0.5);
			glRotatef(270, 0, 1, 0);
			glTranslatef(-0.5, 0, -0.5);
			silla(i);
			break;
		case 5: //Mesa
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			mesa(i);
			break;
		case 15: //Mesa girada
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(1, 0, 1);
			glRotatef(-90, 0, 1, 0);
			glTranslatef(-1, 0, 0);
			mesa(i);
			break;
		case 6: //Cuadro lateral
			glTranslatef(0, arrayPosiciones[i][1], arrayPosiciones[i][0]);
			cuadro(i);
			break;
		case 16: //Cuadro fondo
			glTranslatef((ancho - 1) - arrayPosiciones[i][0], arrayPosiciones[i][1], 0);
			glTranslatef(0, 0, 0.1);
			glRotatef(90, 0, 1, 0);
			cuadro(i);
			break;
		case 7: //Cuadro lateral
			glTranslatef(0, arrayPosiciones[i][1], arrayPosiciones[i][0]);
			cuadroV(i);
			break;
		case 17: //Cuadro fondo
			glTranslatef((ancho - 1) - arrayPosiciones[i][0], arrayPosiciones[i][1], 0);
			glTranslatef(0, 0, 0.1);
			glRotatef(90, 0, 1, 0);
			cuadroV(i);
			break;
		case 8:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			planta(i);
			break;
		}
		glPopMatrix();
	}
	actualizarOcupacion();

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
		//Se coloca la figura predeterminada
		switch (arrayFiguras[i]) {
		case 0: //Figura
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFiguraC(color, i, 0);
			break;
		case 1://Figura
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFiguraC(color, i, 90);
			break;
		case 2://Figura
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFiguraC(color, i, 180);
			break;
		case 3://Figura
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			pintarFiguraC(color, i, 270);
			break;
		case 4: //Silla
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			sillaC(color, i);
			break;
		case 14:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(0.5, 0, 0.5);
			glRotatef(90, 0, 1, 0);
			glTranslatef(-0.5, 0, -0.5);
			sillaC(color, i);
			break;
		case 24:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(0.5, 0, 0.5);
			glRotatef(180, 0, 1, 0);
			glTranslatef(-0.5, 0, -0.5);
			sillaC(color, i);
			break;
		case 34:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(0.5, 0, 0.5);
			glRotatef(270, 0, 1, 0);
			glTranslatef(-0.5, 0, -0.5);
			sillaC(color, i);
			break;
		case 5: //Mesa
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			mesaC(color, i);
			break;
		case 15: //Mesa girada
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			glTranslatef(1, 0, 1);
			glRotatef(-90, 0, 1, 0);
			glTranslatef(-1, 0, 0);
			mesaC(color, i);
			break;
		case 6: //Cuadro lateral
			glTranslatef(0, arrayPosiciones[i][1], arrayPosiciones[i][0]);
			cuadroC(color, i);
			break;
		case 16: //Cuadro fondo
			glTranslatef((ancho - 1) - arrayPosiciones[i][0], arrayPosiciones[i][1], 0);
			glTranslatef(0, 0, 0.1);
			glRotatef(90, 0, 1, 0);
			cuadroC(color, i);
			break;
		case 7: //Cuadro lateral
			glTranslatef(0, arrayPosiciones[i][1], arrayPosiciones[i][0]);
			cuadroVC(color, i);
			break;
		case 17: //Cuadro fondo
			glTranslatef((ancho - 1) - arrayPosiciones[i][0], arrayPosiciones[i][1], 0);
			glTranslatef(0, 0, 0.1);
			glRotatef(90, 0, 1, 0);
			cuadroVC(color, i);
			break;
		case 8:
			glTranslatef(arrayPosiciones[i][0], 0, arrayPosiciones[i][1]);
			plantaC(color, i);
			break;
		}
		glPopMatrix();
	}
	glPopMatrix();
	actualizarOcupacion();
	glPopMatrix(); // restaura la matriz de modelado
}
// Apartado E: Método/s para indicar el objeto seleccionado



