#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl\glut.h>

#include "SolarSystemController.h"
#include "SOIL.h"
#include "Planet.h"
#include "Orbit.h"

const int SCALE_FACTOR = 1000000;


GLUquadric * quadric;

Planet * mercury;
Planet * venus;
Planet * earth;
Planet * moon; //maybe this needs a unique object
Planet * mars;
Planet * jupiter;
Planet * saturn;
Planet * uranus;
Planet * neptune;

GLuint sunTex;
GLuint skyTex;
GLuint saturnRingsTex;
GLuint clouds;
GLfloat mat_emission[2][4] = {{.0f, .0f, .0f, .0f}, {1.0f, 1.0f, 1.0f, 1.0f}};

//private methods
void initializePlanets(void);
void loadTextures(void);
void drawSun(void);
void drawSky(void);
void drawSaturnRing(void);
void drawEarthAtmosphere(void);
GLuint loadTexture(char * filename);
GLuint loadTexture_debug(char * filename);

void SSController_Initialize(void){
	// set up quadric object and turn on FILL draw style for it
	quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricDrawStyle(quadric, GLU_FILL);
	// turn on texture coordinate generator for the quadric
	gluQuadricTexture(quadric, GL_TRUE);

	//Create Planets
	initializePlanets();

	//Get Textures
	loadTextures();
}

void SSController_Delete(void){
	Planet_delete(mercury);
	Planet_delete(venus);
	Planet_delete(earth);
	Planet_delete(moon); 
	Planet_delete(mars);
	Planet_delete(jupiter);
	Planet_delete(saturn);
	Planet_delete(uranus);
	Planet_delete(neptune);

}

void SSController_Draw(void){
	drawSun();
	drawSky();

	Planet_draw(mercury,quadric);
	Planet_draw(venus,quadric);

	Planet_draw(earth,quadric);
	Orbit_setPosition(Planet_getOrbit(moon), Planet_getPosX(earth), 0, Planet_getPosZ(earth));
	
	Planet_draw(moon,quadric);

	Planet_draw(mars,quadric);
	Planet_draw(jupiter,quadric);
	Planet_draw(saturn,quadric);
	Planet_draw(uranus,quadric);
	Planet_draw(neptune,quadric);

	drawSaturnRing();
	drawEarthAtmosphere();

}

void SSController_Update(float dt) {
	Planet_update(mercury, dt);
}

//----------------- private ---------------

void drawEarthAtmosphere(void){
	Vector3 pos;
	glPushMatrix();
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

		glBindTexture(GL_TEXTURE_2D, clouds);
		pos = Planet_getPosition(earth);
		//glTranslatef(0.0,1.f,0.f);
		
		glTranslatef(pos.x, pos.y, pos.z);
		glRotated(90,1,0,0);
		//glColor4f(255,0, 0, 100);
		gluSphere(quadric,Planet_getRadius(earth)+0.01, 32,32);
		gluSphere(quadric,Planet_getRadius(earth)+0.02, 32,32);
		/*
		*/
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	glPopMatrix();
}

void drawSaturnRing(void){
	Vector3 pos;
	glPushMatrix();
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency

		glBindTexture(GL_TEXTURE_2D, saturnRingsTex);
		pos = Planet_getPosition(saturn);
		//glTranslatef(0.0,1.f,0.f);
		
		glTranslatef(pos.x, pos.y, pos.z);
		glRotated(70,1,0,0);
		glScaled(15,15,15);

		//glColor4f(6,0, 0, 0.5);

		//gluDisk(quadric,1,2,10,3);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
		/*
		*/
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	glPopMatrix();
}

void drawSun(void){
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, sunTex);
		glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission[1]);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glRotatef(90,1,0,0);
		glScaled(6,6,6);
		glTranslatef(0.0f, 0.0f, 0.0f);
		//glColor4f(6,0, 0, 0.5);
		gluSphere(quadric, 1.2, 32,32);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission[0]);
	glPopMatrix();
}

void drawSky(void){
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, skyTex);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glTranslatef(0.0f, 0.0f, 0.0f);
		//glColor4f(6,0, 0, 0.5);
		gluSphere(quadric, 1000, 32,32);
	glPopMatrix();
}

void initializePlanets(void){
	mercury = Planet_new();
	venus = Planet_new();
	earth = Planet_new();
	moon= Planet_new(); //maybe this needs a unique object
	mars= Planet_new();
	jupiter= Planet_new();
	saturn= Planet_new();
	uranus= Planet_new();
	neptune= Planet_new();

	//mercury
	Planet_setName(mercury, "mercury");
	Planet_setRadius(mercury,  .24397);
	Planet_setPosition(mercury, 0,0,0);
	Planet_setInclination(mercury, 7.0);
	Planet_setLengthOfYear(mercury, 87.97);
	Orbit_setPosition(Planet_getOrbit(mercury), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(mercury), 5.7909227);
	Orbit_setSemiMajor(Planet_getOrbit(mercury), 0.71);
	Orbit_setSemiMinor(Planet_getOrbit(mercury), 0.69);

	//venus
	Planet_setName(venus, "venus");
	Planet_setRadius(venus,  0.60518);
	Planet_setPosition(venus, 0,0,0);
	Planet_setInclination(venus, 3.39);
	Planet_setLengthOfYear(venus, 224.70);
	Orbit_setPosition(Planet_getOrbit(venus), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(venus), 10.820948);
	Orbit_setSemiMajor(Planet_getOrbit(venus), 0.707115);
	Orbit_setSemiMinor(Planet_getOrbit(venus), 0.707099);


	//earth
	Planet_setName(earth, "Earth");
	Planet_setRadius(earth,  0.6371);
	Planet_setPosition(earth,0,0,0);
	Planet_setInclination(earth, 0.00005);
	Planet_setLengthOfYear(earth, 365.24);
	Orbit_setPosition(Planet_getOrbit(earth), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(earth), 14.959829);
	Orbit_setSemiMajor(Planet_getOrbit(earth), 0.707157);
	Orbit_setSemiMinor(Planet_getOrbit(earth), 0.707056);

	//moon
	Planet_setName(moon, "Moon");
	Planet_setRadius(moon,  0.17375);
	Planet_setPosition(moon,0,0,0);
	Planet_setInclination(moon, 1);
	Planet_setLengthOfYear(moon, 100);
	Orbit_setPosition(Planet_getOrbit(moon), 100,0,100);
	Orbit_setRadius(Planet_getOrbit(moon), 1);
	Orbit_setSemiMinor(Planet_getOrbit(moon), 1);
	Orbit_setSemiMajor(Planet_getOrbit(moon), 1);

	//mars
	Planet_setName(mars, "Mars");
	Planet_setRadius(mars,  0.33895);
	Planet_setPosition(mars,0,0,0);
	Planet_setInclination(mars, 1.85);
	Planet_setLengthOfYear(mars, 686.98);
	Orbit_setPosition(Planet_getOrbit(mars), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(mars), 22.794382);
	Orbit_setSemiMajor(Planet_getOrbit(mars), 0.708653);
	Orbit_setSemiMinor(Planet_getOrbit(mars), 0.705557);

	//jupiter
	Planet_setName(jupiter, "Jupiter");
	Planet_setRadius(jupiter,  6.9911);
	Planet_setPosition(jupiter,0,0,0);
	Planet_setInclination(jupiter, 1.304);
	Planet_setLengthOfYear(jupiter, 4332.82 );
	Orbit_setPosition(Planet_getOrbit(jupiter), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(jupiter), 77.8340821);
	Orbit_setSemiMajor(Planet_getOrbit(jupiter), 0.707521);
	Orbit_setSemiMinor(Planet_getOrbit(jupiter), 0.706692);

	//saturn
	Planet_setName(saturn, "Saturn");
	Planet_setRadius(saturn,  5.8232);
	Planet_setPosition(saturn,0,0,0);
	Planet_setInclination(saturn, 2.49 );
	Planet_setLengthOfYear(saturn, 10755.70 );
	Orbit_setPosition(Planet_getOrbit(saturn), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(saturn), 142.666642);
	Orbit_setSemiMajor(Planet_getOrbit(saturn), 0.707619);
	Orbit_setSemiMinor(Planet_getOrbit(saturn), 0.706594);

	//uranus
	Planet_setName(uranus, "uranus");
	Planet_setRadius(uranus,  2.5362);
	Planet_setPosition(uranus,0,0,0);
	Planet_setInclination(uranus, 0.77 );
	Planet_setLengthOfYear(uranus, 30687.15);
	Orbit_setPosition(Planet_getOrbit(uranus), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(uranus), 287.065819);
	Orbit_setSemiMajor(Planet_getOrbit(uranus), 0.707502);
	Orbit_setSemiMinor(Planet_getOrbit(uranus), 0.706711);

	//neptune
	Planet_setName(neptune, "neptune");
	Planet_setRadius(neptune,  2.4622);
	Planet_setPosition(neptune,0,0,0);
	Planet_setInclination(neptune, 1.77 );
	Planet_setLengthOfYear(neptune, 60190.03 );
	Orbit_setPosition(Planet_getOrbit(neptune), 0, 0, 0);
	Orbit_setRadius(Planet_getOrbit(neptune), 449.839644);
	Orbit_setSemiMajor(Planet_getOrbit(neptune), 0.70712);
	Orbit_setSemiMinor(Planet_getOrbit(neptune), 0.707094);
}

void loadTextures(void){
#ifdef _DEBUG
	GLuint mercuryTex = loadTexture_debug("mercury.tga");
	GLuint venusTex = loadTexture_debug("venus.tga");
	GLuint earthTex = loadTexture_debug("EarthMap.jpg");
	GLuint moonTex = loadTexture_debug("moon.tga");
	GLuint marsTex = loadTexture_debug("mars.tga");
	GLuint jupiterTex = loadTexture_debug("jupiter.tga");
	GLuint saturnTex = loadTexture_debug("saturn.tga");
	GLuint uranusTex = loadTexture_debug("uranus.tga");
	GLuint neptuneTex = loadTexture_debug("neptune.tga");
	sunTex = loadTexture_debug("sun.tga");
	skyTex = loadTexture_debug("galaxy.tga");
	saturnRingsTex = loadTexture_debug("rings.tga");
	clouds = loadTexture_debug("clouds.tga");
#else
	GLuint mercuryTex = loadTexture("mercury.tga");
	GLuint venusTex = loadTexture("venus.tga");
	GLuint earthTex = loadTexture("EarthMap.jpg");
	GLuint moonTex = loadTexture("moon.tga");
	GLuint marsTex = loadTexture("mars.tga");
	GLuint jupiterTex = loadTexture("jupiter.tga");
	GLuint saturnTex = loadTexture("saturn.tga");
	GLuint uranusTex = loadTexture("uranus.tga");
	GLuint neptuneTex = loadTexture("neptune.tga");
	sunTex = loadTexture("sun.tga");
	skyTex = loadTexture("galaxy.tga");
	saturnRingsTex = loadTexture("rings.tga");
	clouds = loadTexture("clouds.tga");
#endif

	Planet_setTexture(mercury, mercuryTex);
	Planet_setTexture(venus, venusTex);
	Planet_setTexture(earth, earthTex);
	Planet_setTexture(moon, moonTex);
	Planet_setTexture(mars, marsTex);
	Planet_setTexture(jupiter, jupiterTex);
	Planet_setTexture(saturn, saturnTex);
	Planet_setTexture(uranus, uranusTex);
	Planet_setTexture(neptune, neptuneTex);
}

GLuint loadTexture(char * filename){
	GLuint texture = 0;
	char path [30]="textures\\";

	strcat(path,filename);
	texture = SOIL_load_OGL_texture(path,
								SOIL_LOAD_AUTO,
								SOIL_CREATE_NEW_ID,
								0);
	
	if(texture == NULL){
		exit(0); //Need to have a better error handling
	}
		// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT to fix the line bug, to see the bug, change it to GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return texture;
}

GLuint loadTexture_debug(char * filename){
	GLuint texture = 0;
	char path [30]="textures\\";

#ifdef _DEBUG
	int start,end;
	start = glutGet(GLUT_ELAPSED_TIME);
#endif

	strcat(path,filename);
	texture = SOIL_load_OGL_texture(path,
								SOIL_LOAD_AUTO,
								SOIL_CREATE_NEW_ID,
								SOIL_FLAG_INVERT_Y
								);
#ifdef _DEBUG
	end=glutGet(GLUT_ELAPSED_TIME);
	//after the user defined function does its work
	printf("Texture %s load time> %dms\n", filename, (end-start));
#endif


	if(texture == NULL){
#ifdef _DEBUG
		printf("\nSolarSystemController> Failed to load %s texture\n", filename);
#endif
		exit(0); //Need to have a better error handling
	}
		// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_REPEAT to fix the line bug, to see the bug, change it to GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	return texture;
}
