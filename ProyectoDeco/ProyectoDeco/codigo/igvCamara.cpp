#include <GL/glut.h>
#include <math.h>

#include "igvCamara.h"

// Metodos constructores

igvCamara::igvCamara () {}

igvCamara::~igvCamara () {}

igvCamara::igvCamara(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V) {
	position = _P0;
	target = _r;
	up = _V;

	tipo = _tipo;
}


// Metodos publicos 
void igvCamara::set(igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V) {
	position = _P0;
	target = _r;
	up = _V;
}
void igvCamara::set(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
			                                double _xwmin, double _xwmax, double _ywmin, double _ywmax, double _znear, double _zfar) {
	tipo = _tipo;

	position = _P0;
	target = _r;
	up = _V;

	xwmin = _xwmin;
	xwmax = _xwmax;
	ywmin = _ywmin;
	ywmax = _ywmax;
	znear = _znear;
	zfar = _zfar;
}

void igvCamara::set(tipoCamara _tipo, igvPunto3D _P0, igvPunto3D _r, igvPunto3D _V,
			                         double _angulo, double _raspecto, double _znear, double _zfar) {
	tipo = _tipo;

	position = _P0;
	target = _r;
	up = _V;

	angulo = _angulo;
	raspecto = _raspecto;
	znear = _znear;
	zfar = _zfar;
}


void igvCamara::aplicar(void) {
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	if (tipo==IGV_PARALELA) {
		glOrtho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	}
	if (tipo==IGV_FRUSTUM) {
		glFrustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	}
	if (tipo==IGV_PERSPECTIVA) {
		gluPerspective(angulo,raspecto,znear,zfar);
	}

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(position[X], position[Y], position[Z], target[X], target[Y], target[Z], up[X], up[Y], up[Z]);
	gluLookAt(position[X],position[Y],position[Z], target[X]+ position[X],target[Y]+ position[Y],target[Z]+ position[Z], up[X],up[Y],up[Z]);
}



void igvCamara::zoom(double factor) {

}
