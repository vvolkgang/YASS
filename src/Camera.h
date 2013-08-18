#ifndef CAMERA_H
#define CAMERA_H

typedef struct _camera Camera;

struct _camera
{
	float angle, anglev; // Ângulo relativo ao eixo dos Z's, no plano ZX
	float deltaAngle, deltaAnglev, deltaMove;
	float x, y, z; // Posição da câmara (centro-de-massa)
	float lx, ly, lz; // Vector do sentido de deslocamento da câmara
	float upx, upy, upz;
	float deltaSpeed;
	float speed; // Constante de velocidade de deslocamento
	float zNear, zFar; //near plane e far plane do viewing frustum
	float fov; // field of view
} ;

/********
 * Constructor
 ********/
Camera * Camera_new(void);
Camera * Camera_new_initialized(void);

/********
 * Destructor
 ********/
void Camera_delete(Camera * camera);


void Camera_initialize(Camera * camera);
void Camera_updateFPSPosition(Camera * camera, float dt);
#endif