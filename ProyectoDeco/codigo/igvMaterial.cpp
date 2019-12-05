#include "igvMaterial.h"

// Metodos constructores 

igvMaterial::igvMaterial() {

}

igvMaterial::~igvMaterial() {

}

igvMaterial::igvMaterial(const igvMaterial& p) {	//de copia
	Ka = p.Ka;
	Kd = p.Kd;
	Ks = p.Ks;
	Ns = p.Ns;
}

igvMaterial::igvMaterial(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns) {
	Ka = _Ka;
	Kd = _Kd;
	Ks = _Ks;
	Ns = _Ns;
}

// Metodos publicos 

void igvMaterial::aplicar(void) {

	// APARTADO C
	// Aplicar los valores de los atributos del objeto igvMaterial:
	// - coeficiente de reflexión de la luz ambiental
	// - coeficiente de reflexión difuso
	// - coeficiente de reflesión especular
	// - exponente de Phong
	float ambiente[4] = { (float)Ka[0], (float)Ka[1], (float)Ka[2],(float)Ka[3] };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);

	float difuso[4] = { (float)Kd[0], (float)Kd[1], (float)Kd[2],(float)Kd[3] };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, difuso);

	float espectacular[4] = { (float)Ks[0], (float)Ks[1], (float)Ks[2],(float)Ks[3] };
	glMaterialfv(GL_FRONT, GL_SPECULAR, espectacular);

	glMaterialf(GL_FRONT, GL_SHININESS, Ns);

	// establecer como color de emisión (0.0, 0.0, 0.0) (el objeto no es una fuente de luz)
	GLfloat emision[3] = { 0,0,0 };
	glMaterialfv(GL_FRONT,GL_EMISSION,emision);


}

void igvMaterial::set(igvColor _Ka, igvColor _Kd, igvColor _Ks, double _Ns) {
	Ka = _Ka;
	Kd = _Kd;
	Ks = _Ks;
	Ns = _Ns;
}



