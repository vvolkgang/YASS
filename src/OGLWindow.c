#include <gl\glut.h>
#include "OGLWindow.h"


#define WIN_WIDTH 512
#define WIN_HEIGHT 512
#define WIN_WIDTH_GM 1366
#define WIN_HEIGHT_GM 738

#define WIN_POS_X 100

#define WIN_POS_Y 100

#define BPP 32 //bitsPerPixel: 16 or 32
#define HERTZ 60 //videoRate


const char * _gameModeString = "1366x768:32"; //Fixed, needs to be dynamic


//[ width "x" height ][ ":" bitsPerPixel ][ "@" videoRate ]
const char * OGLWindow_getGameModeString(void){
	return _gameModeString;
}

void OGLWindow_initGameMode(void){
	OGLWINDOW.width = WIN_WIDTH_GM;
	OGLWINDOW.height = WIN_HEIGHT_GM;
	OGLWINDOW.posX = 0;
	OGLWINDOW.posY = 0;

	glutEnterGameMode();
}


void OGLWindow_initWindowMode(void){
	OGLWINDOW.width = WIN_WIDTH;
	OGLWINDOW.height = WIN_HEIGHT;
	OGLWINDOW.posX = WIN_POS_X;
	OGLWINDOW.posY = WIN_POS_Y;

	glutInitWindowPosition(OGLWINDOW.posX, OGLWINDOW.posY);
	glutInitWindowSize(OGLWINDOW.width, OGLWINDOW.height);
	glutCreateWindow("SolarSystem");
}

void OGLWindow_GameMode_to_WindowMode(void){
	glutLeaveGameMode();

	OGLWindow_initWindowMode();
}

void OGLWindow_setOrthographicProjection(void)
{
	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection (invert the y axis, down is positive)
	gluOrtho2D(0, OGLWINDOW.width, OGLWINDOW.height/2, 0);
	glMatrixMode(GL_MODELVIEW);
}


void OGLWindow_resetPerspectiveProjection(void)
{
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

