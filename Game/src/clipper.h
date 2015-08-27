#ifndef _CLIPPER_
#define _CLIPPER_

#include <math.h>
#include "includes.h"
#include <GL/glut.h>

class Clipper
{
public:
	float frustum[6][4]; //6 planos de 4 floats. 
	void ExtractFrustum(float view_matrix[16], float projection_matrix[16]);
	bool PointInFrustum( float x, float y, float z ); //Si esta punto dentro o fuera.
	bool SphereInFrustum( float x, float y, float z, float radius ); //si esfera esta dentro o fuera. 
};

#endif