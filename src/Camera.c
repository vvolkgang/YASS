#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "Camera.h"


// -------------
//
// Constructor
//
// -------------
Camera * Camera_new(void) {
	Camera * novo;

	if ((novo = ( Camera*)malloc(sizeof(Camera))) == NULL) { 
		exit(1);
	}
	memset(novo,0xda,sizeof( Camera));

	return novo;
}

Camera * Camera_new_initialized(void) {
	Camera * novo;

	if ((novo = ( Camera*)malloc(sizeof(Camera))) == NULL) { 
		exit(1);
	}
	
	Camera_initialize(novo);

	return novo;
}


// -------------
//
// Destructor
//
// -------------
void Camera_delete( Camera * camera) {

	memset(camera,0xdd,sizeof( Camera));
	free(camera);
}

void Camera_initialize(Camera * camera){
	camera->angle = 0.0f;
	camera->anglev = 0.0f;
	camera->deltaAngle = 0.0f;
	camera->deltaAnglev = 0.0f;
	camera->deltaMove = 0.0f;
	camera->x = 0.0f;
	camera->y = 10.0f;
	camera->z = 5.0f;
	camera->lx = 0.0f;
	camera->ly = 0.0f;
	camera->lz = -1.0f;
	camera->upx = 0.0f;
	camera->upy = 1.0f;
	camera->upz = 0.0f;
	camera->speed = 0.1f;
	camera->deltaSpeed = 0.0f;
	camera->zNear = 0.5;
	camera->zFar = 2000.0f;
	camera->fov = 60.0f;
}

void Camera_updateFPSPosition(Camera * camera, float dt)
{
	if(camera->deltaSpeed)
	{
		camera->speed += camera->deltaSpeed;
		if(camera->speed < 0.0f) camera->speed = 0.0f ;
	}

	if(camera->deltaMove)
	{
		// x = x0 + (Amplitude * Direcção) * Velocidade
		camera->x = camera->x + camera->deltaMove * (camera->lx) * camera->speed;
		camera->y = camera->y + camera->deltaMove * (camera->ly) * camera->speed;
		camera->z = camera->z + camera->deltaMove * (camera->lz) * camera->speed;
	}

	if(camera->deltaAngle || camera->deltaAnglev)
	{
		camera->angle += camera->deltaAngle;
		camera->anglev += camera->deltaAnglev;
		camera->lx = sin(camera->angle) * cos(camera->anglev);
		camera->ly = sin(camera->anglev);
		camera->lz = -cos(camera->angle) * cos(camera->anglev) ;
	}

	if(cos(camera->anglev) < 0.0f)
	{
		camera->upy = -1.0f;
	}
	else
	{
		camera->upy = 1.0f;
	}
}