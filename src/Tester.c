#include "Tester.h"

#ifdef _DEBUG
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Planet.h"
#include "Global.h"
#include "Camera.h"
#include "FPS.h"
#include "SolarSystemController.h"

//private
void testPlanet(void);
void testCamera(void);
void testFPS(void);

void objectCreationTest(void){
	printf("#TEST - Creation & destruction\n\n");
	testPlanet();
	testCamera();
	testFPS();
	//testSolarSystemController();
	printf("\n#TEST - Creation & destruction - Sucess!\n\n");
}

//--------------- private ---------------
void testSolarSystemController(void){
	SSController_Initialize();
	SSController_Delete();
	printf("SolarSystemController>\t\t\tOK\n");
}

void testFPS(void){
	FPS * fps;
	fps = FPS_new();
	fps->frame = 12;
	FPS_delete(fps);
	printf("FPS>\t\t\tOK\n");

}

void testPlanet(void){
	Planet * earth;

	earth = Planet_new();
	Planet_setName(earth, "Terra");
	Planet_getName(earth);
	Planet_delete(earth);

	printf("Planet>\t\t\tOK\n");
}

void testCamera(void){
	Camera * cam;

	cam = Camera_new_initialized();
	cam->angle = 300.0f;
	Camera_delete(cam);
	
	printf("Camera>\t\t\tOK\n");
}
#endif