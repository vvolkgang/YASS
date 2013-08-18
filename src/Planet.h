/*
 *  Planet.h
 */

#ifndef PLANET_H
#define PLANET_H

#include <stdio.h>
#include <gl\glut.h>
#include "Vector3.h"
#include "Orbit.h"



typedef struct _planet Planet;

/********
 * Constructor
 ********/
Planet * Planet_new(void);
Planet * Planet_newWithData(char * name);


/********
 * Destructor
 ********/
void Planet_delete(Planet * planet);

/********
 * GET'rs
 ********/
char * Planet_getName( Planet * planet);
GLuint Planet_getTexture(Planet * planet);
GLdouble Planet_getRadius(Planet * planet);
Orbit * Planet_getOrbit(Planet * planet);
float Planet_getPosX(Planet * planet);
float Planet_getPosY(Planet * planet);
float Planet_getPosZ(Planet * planet);
Vector3 Planet_getPosition(Planet * planet);


/*********
 * SET'rs
 ********/
void Planet_setName(Planet * planet, char * name);
void Planet_setTexture(Planet * planet, GLuint texture);
void Planet_setRadius(Planet * planet, GLdouble radius);
void Planet_setVector3(Planet * planet, Vector3 vector);
void Planet_setPosition(Planet * planet, GLfloat x, GLfloat y, GLfloat z);
void Planet_setInclination(Planet * planet, GLdouble inclination);
void Planet_setLengthOfYear(Planet * planet, GLdouble length);
/*********
 * General methods
 ********/
void Planet_update(Planet * planet, float dt);
void Planet_draw(Planet * planet, GLUquadric * quadric);

void Planet_setOrbitVisibility(int vis);
int Planet_getOrbitVisibility();

#endif /* PLANET_H */