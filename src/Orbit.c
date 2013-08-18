
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Global.h"
#include "Orbit.h"
#include "Vector3.h"




struct _orbit{
	Vector3 position;
	GLuint texture;
	GLdouble radius;
	GLdouble semimajor;
	GLdouble semiminor;
};



// -------------
//
// Constructor
//
// -------------
Orbit * Orbit_new(void) {
	Orbit * novo;
	char * name;
	if ((novo = ( Orbit*)malloc(sizeof(Orbit))) == NULL) { 
		exit(1);
	}
	memset(novo,0xda,sizeof( Orbit));

	return novo;
}

// -------------
//
// Destructor
//
// -------------
void Orbit_delete( Orbit * orbit) {
	
	memset(orbit,0xdd,sizeof( Orbit));
	free(orbit);
}


// -------------
//
// GET'rs
//
// -------------

GLdouble Orbit_getRadius(Orbit * orbit){
	return orbit->radius;
}

Vector3 Orbit_getPosition(Orbit * orbit, float time){
	Vector3 pos = Orbit_getCurrentPosition(orbit);

	pos.x = (cos(time) / orbit->semiminor) * orbit->radius;
	pos.z = (sin(time) / orbit->semimajor) * orbit->radius;
	pos.x += orbit->position.x;
	pos.z += orbit->position.z;

	return pos;
}

Vector3 Orbit_getCurrentPosition(Orbit * orbit){
	return orbit->position;
}
// -------------
//
// SET'rs
//
// -------------

void Orbit_setRadius(Orbit * orbit, GLdouble radius){
	orbit->radius = radius;
}

void Orbit_setVector3(Orbit * orbit, Vector3 vector){
	orbit->position = vector;
}

void Orbit_setPosition(Orbit * orbit, GLfloat x, GLfloat y, GLfloat z){
	orbit->position.x = x;
	orbit->position.y = y;
	orbit->position.z = z;
}

void Orbit_setSemiMinor(Orbit * orbit, GLdouble length) {
	orbit->semiminor = length;
}
void Orbit_setSemiMajor(Orbit * orbit, GLdouble length) {
	orbit->semimajor = length;
}


// -------------
//
// General methods
//
// -------------

void Orbit_draw(Orbit * orbit, GLUquadric * quadric){
	float x = 0;
	float z = 0;
	float i = 0;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
		for (i; i < (3.14 * 2); i += 3.14 / 180) {
			x = (cos(i) / orbit->semiminor) * orbit->radius;
			z = (sin(i) / orbit->semimajor) * orbit->radius;
			x+=orbit->position.x;
			z+=orbit->position.z;
			glVertex3f(x, 0, z);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}
