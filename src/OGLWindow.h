#ifndef OGLWINDOW_H
#define OGLWINDOW_H
struct
{
	int width, height;
	int posX, posY;
} OGLWINDOW;

const char * OGLWindow_getGameModeString(void);
void OGLWindow_initWindowMode(void);
void OGLWindow_initGameMode(void);
void OGLWindow_setOrthographicProjection(void);
void OGLWindow_resetPerspectiveProjection(void);
#endif