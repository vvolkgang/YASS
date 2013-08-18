#ifndef ORBIT_H
#define ORBIT_H

#include <stdio.h>
#include <gl\glut.h>
#include "Vector3.h"



typedef struct _orbit Orbit;

/********
 * Constructor
 ********/
Orbit * Orbit_new(void);

/********
 * Destructor
 ********/
void Orbit_delete(Orbit * orbit);

/********
 * GET'rs
 ********/
Vector3 Orbit_getPosition(Orbit * orbit, float time);
Vector3 Orbit_getCurrentPosition(Orbit * orbit);
GLdouble Orbit_getRadius(Orbit * orbit);

/*********
 * SET'rs
 ********/
void Orbit_setVector3(Orbit * orbit, Vector3 vector);
void Orbit_setPosition(Orbit * orbit, GLfloat x, GLfloat y, GLfloat z);
void Orbit_setRadius(Orbit * orbit, GLdouble radius);
void Orbit_setSemiMinor(Orbit * orbit, GLdouble length);
void Orbit_setSemiMajor(Orbit * orbit, GLdouble length);

/*********
 * General methods
 ********/
void Orbit_draw(Orbit * orbit, GLUquadric * quadric);

#endif