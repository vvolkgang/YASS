#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Global.h"
#include "Planet.h"
#include "Orbit.h"
#include "Vector3.h"


#define SLICES_LOW 16
#define STACKS_LOW 16
#define SLICES_MED 32
#define STACKS_MED 32
#define SLICES_HIGH 128
#define STACKS_HIGH 128

#define SLICES_CURRENT SLICES_MED
#define STACKS_CURRENT STACKS_MED


struct _planet{
	char * name;
	Vector3 position;
	GLuint texture;
	GLdouble radius;
	Orbit* orbit;
	GLdouble inclination;
	GLdouble lengthOfYear;
};

double lifetime = 0.0;
int showOrbits = 1;

// -------------
//
// Constructor
//
// -------------
Planet * Planet_new(void) {
	Planet * novo;
	char * name;
	if ((novo = ( Planet*)malloc(sizeof(Planet))) == NULL) { 
		exit(1);
	}
	memset(novo,0xda,sizeof( Planet));


	if ((novo->name = ( char*)malloc(CHARBUFFSIZE)) == NULL) { 
		exit(1);
	}

	novo->orbit = Orbit_new();

	return novo;
}

Planet * Planet_newWithData(char * name){
	Planet * planetNovo;

	planetNovo = Planet_new();

	Planet_setName(planetNovo, name);

	planetNovo->orbit = Orbit_new();

	return planetNovo;
}

// -------------
//
// Destructor
//
// -------------
void Planet_delete( Planet * planet) {
	
	free(planet->name);

	memset(planet,0xdd,sizeof( Planet));
	free(planet);
}


// -------------
//
// GET'rs
//
// -------------

char * Planet_getName( Planet * planet){
	return planet->name;
}

GLuint Planet_getTexture(Planet * planet){
	return planet->texture;
}

GLdouble Planet_getRadius(Planet * planet){
	return planet->radius;
}

float Planet_getPosX(Planet * planet) {
	Orbit *orb = Planet_getOrbit(planet);

	return planet->position.x;
}
float Planet_getPosY(Planet * planet) {
	Orbit *orb = Planet_getOrbit(planet);

	return planet->position.x;
}
float Planet_getPosZ(Planet * planet) {
	Orbit *orb = Planet_getOrbit(planet);

	return planet->position.z;
}


Vector3 Planet_getPosition(Planet * planet) {

	return planet->position;
}
// -------------
//
// SET'rs
//
// -------------

void Planet_setName( Planet * planet, char * name){
	strcpy(planet->name, name);
}

void Planet_setTexture(Planet * planet, GLuint texture){
	planet->texture = texture;
}

void Planet_setRadius(Planet * planet, GLdouble radius){
	planet->radius = radius;
}

void Planet_setVector3(Planet * planet, Vector3 vector){
	planet->position = vector;
}

void Planet_setPosition(Planet * planet, GLfloat x, GLfloat y, GLfloat z){
	planet->position.x = x;
	planet->position.y = y;
	planet->position.z = z;
}

void Planet_setInclination(Planet * planet, GLdouble inclination) {
	planet->inclination = inclination;
}

void Planet_setLengthOfYear(Planet * planet, GLdouble length) {
	planet->lengthOfYear = length;
}

// -------------
//
// General methods
//
// -------------

void Planet_update(Planet * planet, float dt)
{
	lifetime += dt;
}

void Planet_draw(Planet * planet, GLUquadric * quadric){
	GLdouble dist = Orbit_getRadius(Planet_getOrbit(planet));
	Vector3 pos = Orbit_getPosition(Planet_getOrbit(planet), (lifetime / planet->lengthOfYear) * (2 * 3.14));
	planet->position = pos;

	if(showOrbits == 1)
		Orbit_draw(planet->orbit, quadric);

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, planet->texture);
		glTranslatef(pos.x, pos.y, pos.z);
		glRotated(lifetime * 360, 0, 1, 0);
		glRotated(-90 + planet->inclination, 1, 0, 0);
		gluSphere(quadric,planet->radius,SLICES_CURRENT,STACKS_CURRENT);
	glPopMatrix();
}

Orbit * Planet_getOrbit(Planet * planet)
{
	return planet->orbit;
}

void Planet_setOrbitVisibility(int vis) {
	showOrbits = vis;
}

int Planet_getOrbitVisibility() { return showOrbits; }
