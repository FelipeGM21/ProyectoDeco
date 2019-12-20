#include <cstdlib>
#include <stdio.h>

#include "igvEscena3D.h"
#include "igvPunto3D.h"
#include "igvMaterial.h"
#include "igvTextura.h"
#include <String>

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

static void
drawBox(GLfloat size, GLenum type, const char*archivo)
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
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void glutSolidCube(GLfloat size, const char*archivo)
{
	drawBox(size, GL_QUADS, archivo);
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
	glScalef(1, 1.3, 2.2);
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

void igvEscena3D::silla() {
	
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
	glTranslatef(0.15,0.28,0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	//pata derecha trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25,  archivo);
	glPopMatrix();

	//pata derecha delantera
	glPushMatrix();
	glTranslatef(0.855, 0.28, 0.125);
	glScalef(0.5, 2.1, 0.5);
	glRotatef(90,0,0,1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	//pata izquierda delantera
	glPushMatrix();
	glTranslatef(0.855, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	//asiento
	glPushMatrix();
	glTranslatef(0.5, 0.6, 0.5);
	glScalef(3.4, 0.5, 3.5);
	glRotatef(90, 1, 0, 0);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	//
	glPushMatrix();
	glTranslatef(0.15, 0.98, 0.88);
	glScalef(0.5, 2.5, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 0.98, 0.125);
	glScalef(0.5, 2.5, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 1, 0.5);
	glScalef(0.5, 0.5, 3.5);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.15, 1.3, 0.5);
	glScalef(0.5, 0.5, 3.5);
	glRotatef(90, 0, 1, 0);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	glPopMatrix();

}

void igvEscena3D::mesa() {
	igvColor ambient(0.105882f, 0.058824f, 0.113725f, 1.0f);
	igvColor diffuse(0.427451f, 0.470588f, 0.541176f, 1.0f);
	igvColor specular(0.333333f, 0.333333f, 0.521569f, 1.0f);
	double shine = 9.84615f;

	igvMaterial material(ambient,diffuse,specular,shine);
	material.aplicar();

	glPushMatrix();
	
	//glMaterialfv(GL_FRONT, GL_EMISSION, marron);
	const char *archivo = "./mesa.bmp";
	//pata izquierda trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25,archivo);
	glPopMatrix();

	//pata derecha trasera
	glPushMatrix();
	glTranslatef(0.15, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	//pata derecha delantera
	glPushMatrix();
	glTranslatef(1.855, 0.28, 0.125);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	//pata izquierda delantera
	glPushMatrix();
	glTranslatef(1.855, 0.28, 0.88);
	glScalef(0.5, 2.2, 0.5);
	glRotatef(90, 0, 0, 1);
	glutSolidCube(0.25, archivo);
	glPopMatrix();

	//superficie
	glPushMatrix();
	glTranslatef(1, 0.6, 0.5);
	glScalef(7.4, 0.5, 3.5);
	glutSolidCube(0.25, archivo);
	glPopMatrix();
	glPopMatrix();

}

void igvEscena3D::pintar_quad(float div_x, float div_z,const char*archivo) {
	float ini_x = 0.0;
	float ini_z = 0.0;
	float tam_x = 1.0;
	float tam_z = 1.0;

	igvTextura textura(archivo);

	textura.aplicar();
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);

	glTexCoord2f(0.0f,0.0f);glVertex3f(ini_x, 0.0, ini_z);

	glTexCoord2f(1.0f, 0.0f);glVertex3f(ini_x, 0.0, ini_z + tam_z);

	glTexCoord2f(1.0f, 1.0f);glVertex3f(ini_x + tam_x, 0.0, ini_z + tam_z);

	glTexCoord2f(0.0f, 1.0f);glVertex3f(ini_x + tam_x, 0.0, ini_z);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void igvEscena3D::crearSuelo(int tamx, int tamy) {
	igvColor kBlanco = igvColor(1, 1, 1);
	igvColor kNegro = igvColor(0, 0, 0);
	igvColor kDifuso = igvColor(0, 0, 0);
	igvColor kSpecular = igvColor(0, 0, 0);
	igvMaterial blanco = igvMaterial(kBlanco,kDifuso,kSpecular,1);
	igvMaterial negro = igvMaterial(kNegro, kDifuso, kSpecular, 1);
	const char *archivo = "./suelo.bmp";
	
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
			pintar_quad(1, 1,archivo);
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
			pintar_quad(1, 1,archivo);
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

void igvEscena3D::pintar_triangulo3D(const char* archivo){
	GLfloat azul[] = { 0,0,1,1.0 };

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);

	igvTextura textura(archivo);
	textura.aplicar();
	
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 0.0f);glVertex3f(0.0, 0.0, 1.0);

	glTexCoord2f(1.0f, 0.0f);glVertex3f(1.0, 1.0, 1.0);

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

void igvEscena3D::parte_cuadro2(const char* archivo) {
	GLfloat rojo[] = { 1,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
	

	glPushMatrix();
	glTranslatef(0.0,1.0,0.0);
		glPushMatrix();
		glTranslatef(0.075, 0.42, 0.275);
		glScalef(0.6, 2.2, 0.59);
		glRotatef(90, 0, 0, 1);
		glutSolidCube(0.25, archivo);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0, 0.69, 0.35);
		glScalef(0.15, 0.15, 0.15);
		glRotatef(90, 0, 1, 0);
		pintar_triangulo3D(archivo);
		glPopMatrix();
		
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0.5,0);
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

void igvEscena3D::cuadroV(){
	GLfloat rojo[] = { 1,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);

	const char *archivo = "./marco.bmp";
	const char *archivo2 = "./paisaje.bmp";
	const char *fondo1 = "./fondo_parte2.bmp";
	const char *fondo2 = "./fondo_parte1.bmp";

	//izquierda
	glPushMatrix();
	glTranslatef(0.15, 0.85, 2.13);
	glRotatef(180, 0, 1, 0);
	parte_cuadro2(archivo);
	glPopMatrix();

	//derecha
	glPushMatrix();
	glTranslatef(0.0, 0.85, 0.89);
	parte_cuadro2(archivo);
	glPopMatrix();

	//arriba
	glPushMatrix();
	glTranslatef(0.0, 2.89, 1.09);
	glRotatef(90, 1, 0, 0);
	parte_cuadro(archivo);
	glPopMatrix();

	//abajo
	glPushMatrix();
	glTranslatef(0.15, 1.15, 1.09);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	parte_cuadro(archivo);
	glPopMatrix();

	//fondo
	igvTextura textura(fondo1);
	textura.aplicar();
	glPushMatrix();
	glTranslatef(0.05, 0.8, 0.6);
	glScalef(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glPopMatrix();

	igvTextura textura2(fondo2);
	textura2.aplicar();
	glPushMatrix();
	glTranslatef(0.05, 1.4, 0.6);
	glScalef(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
	glPopMatrix();
}

void igvEscena3D::cuadro() {
	GLfloat rojo[] = { 1,0,0,1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, rojo);

	const char *archivo = "./marco.bmp";
	const char *archivo2 = "./paisaje.bmp";

	//izquierda
	glPushMatrix();
	glTranslatef(0.15, 1.1, 2.13);
	glRotatef(180,0,1,0);
	parte_cuadro(archivo);
	glPopMatrix();

	//derecha
	glPushMatrix();
	glTranslatef(0.0, 1.1, 0.89);
	parte_cuadro(archivo);
	glPopMatrix();	

	//arriba
	glPushMatrix();
	glTranslatef(0.0, 2.13, 1.09);
	glRotatef(90,1,0,0);
	parte_cuadro(archivo);
	glPopMatrix();

	//abajo
	glPushMatrix();
	glTranslatef(0.15, 0.9, 1.09);
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	parte_cuadro(archivo);
	glPopMatrix();	

	//fondo

	//glRotatef(90, 0, 0, 1);
	//glScalef(0.4, 0.4, 0.4);
	igvTextura textura(archivo2);
	textura.aplicar();
	glTranslatef(0.05,0.3,0.3);
	glScalef(0.8, 0.8, 0.8);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 2.0, 2.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 2.0, 1.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 1.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, 2.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();

}

void igvEscena3D::planta() {
	GLfloat azul[] = { 0,0,1,1.0 };
	const char *hojas = "./hojas.bmp";
	const char *palo = "./palo.bmp";
	const char *base_maceta = "./maceta.bmp";
	

	glMaterialfv(GL_FRONT, GL_EMISSION, azul);
	glPushMatrix();
	glTranslatef(0.5, 0.9, 0.5);
	glScalef(0.25,3.2, 0.25);
	glutSolidCube(0.5,palo);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.6, 0.5);
	glScalef(0.4, 0.25, 0.4);
	glutSolidCube(1,hojas);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1.3, 0.5);
	glScalef(0.5, 0.25, 0.5);
	glutSolidCube(1,hojas);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 1, 0.5);
	glScalef(0.6, 0.25, 0.6);
	glutSolidCube(1, hojas);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, 0.7, 0.5);
	glScalef(0.7, 0.25, 0.7);
	glutSolidCube(1, hojas);
	glPopMatrix();

	glTranslatef(0.3,0.0,0.3);
	glScalef(0.5, 0.5, 0.5);
	maceta(base_maceta);

}

void igvEscena3D::tierra() {
	const char *tierra_base = "./tierra.bmp";
	igvTextura tierra(tierra_base);

	glBegin(GL_QUADS);
	tierra.aplicar();
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.4, 0.7);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.4, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.4, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.4, 0.7);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glLoadIdentity();
}

void igvEscena3D::maceta(const char* archivo) {
	GLfloat verde[] = { 0,1,0,1.0 };
	GLfloat rojo[] = { 1,0,0,1.0 };
	const char *tierra_base = "./tierra.bmp";

	glMaterialfv(GL_FRONT, GL_EMISSION, verde);
	igvTextura maceta(archivo);

	maceta.aplicar();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 0.0, 0.7);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.2, 0.0, 0.2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.7, 0.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.7, 0.0 ,0.7);

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
	glLoadIdentity();
	glEnd();

	tierra();
}

void igvEscena3D::visualizar() {
	// crear luces
	GLfloat luz0[4] = { 5.0,5.0,5.0,1 }; // luz puntual  
	glLightfv(GL_LIGHT0, GL_POSITION, luz0); // la luz se coloca aquí si permanece fija y no se mueve con la escena
	glEnable(GL_LIGHT0);
	igvPunto3D posicion(1.0, 1.0, 1.0);
	igvColor ambiental(0.0,0.0,0.0,1.0);
	igvColor difuso(1.0, 1.0, 1.0, 1.0);
	igvColor especular(1.0, 1.0, 1.0, 1.0);

	igvFuenteLuz luz(GL_LIGHT0,
		posicion,
		ambiental, difuso, especular,
		1.0, 0.0, 0.0);
	luz.aplicar();
	
	


	// crear el modelo
	glPushMatrix(); // guarda la matriz de modelado

	// se pintan los ejes
	if (ejes) pintar_ejes();

	//glLightfv(GL_LIGHT0,GL_POSITION,luz0); // la luz se coloca aquí si se mueve junto con la escena

///// Apartado B: aquí hay que añadir la visualización del árbol del modelo utilizando la pila de matrices de OpenGL
/////             se recomienda crear una método auxiliar que encapsule todo el código para la visualización
/////             del modelo, dejando aquí sólo la llamada a ese método, así como distintas funciones una para cada
/////			  parte del modelo. 
	
	crearSuelo(3,3);

	glPushMatrix();
	int baldosaX = 1;
	int baldosaZ = 3;
	glTranslatef(baldosaX, 0, baldosaZ);
	//pintarFigura();
	
	glPopMatrix();

	
	
	glPushMatrix();
	glTranslatef(0, 0, 0);
	//glMaterialfv();
	cuadroV();
	//silla();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	planta();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 1);
	//glMaterialfv();
	//silla();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 1);
	//glMaterialfv();
	cuadro();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(1,0,0);
	//silla();
	//mesa();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	crearPared(3,3);
	glPopMatrix();
	glRotatef(-90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	crearPared(3,3);
	glPopMatrix();

	//const char *archivo = "./suelo.bmp";
	////glScalef(0.5, 2.2, 0.5);
	//glRotatef(90, 0, 0, 1);
	//glutSolidCube(1,archivo);

	//visualizar_BV();

	///// En el apartado E habrá que asignar un color de selección al objeto para indicar que la parte seleccionada se
	/////			visualice de un color distinto



	glPopMatrix(); // restaura la matriz de modelado
}


// Apartado E: Método/s para indicar el objeto seleccionado



