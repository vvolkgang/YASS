#include <stdio.h>
#include <gl\glut.h>
#include <gl\gl.h>    
#include <gl\glu.h>
#include <math.h>
#include <AntTweakBar.h>

#include "main.h"


#include "SOIL.h"
#include "Global.h"
#include "Planet.h"
#include "Camera.h"
#include "OGLWindow.h"
#include "FPS.h"
#include "SolarSystemController.h"
#include "GameTime.h"

#ifdef _DEBUG
#include "Tester.h"
#endif

//-------Defines

//-------End Defines

int OGLFONT = (int) GLUT_BITMAP_8_BY_13;


Camera * _camera;
FPS * _fpsCounter;
GameTime * gameTime;
float deltaTime = 0.0f;
float TimeScale = 10.0f;
float days = 0.0f;
int myDL;
int FOG_ON = 0;
int manualCam = 1;
float camdt = 0.0f;
int lastX = 0;
int lastY = 0;

TwBar *mainBar;
float camSpeed = 0.5f;

int orbits = 1;

void initGL(int argc, char** argv);
void initGameTime(void);
void mouseMove(int x, int y);
void mouseClick(int button, int state, int x, int y);

void display(void)
{
#ifdef _DEBUG
	char infotext[255];
#endif
	GameTime_update(gameTime);

	deltaTime = (float)(gameTime->sinceLastDisplay_seconds * TimeScale);
	days += deltaTime;

	/*if(manualCam)
		updateCameraPosition();
	else
	{
		_camera->x = cos(camdt) * 100;
		_camera->z = sin(camdt) * 100;
		_camera->y = 15;

		_camera->lx = -0.1f;
		_camera->ly =  0.0f;
		_camera->lz = -0.1f;
	}*/


	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	
	camdt -= 10 * gameTime->sinceLastDisplay_seconds;
	if(manualCam)
	{
		_camera->speed = camSpeed;
		Camera_updateFPSPosition(_camera, camdt);

		gluLookAt(_camera->x, _camera->y, _camera->z, 
			  _camera->x+_camera->lx, _camera->y+_camera->ly, _camera->z+_camera->lz,
			  _camera->upx, _camera->upy, _camera->upz);
	}
	else
	{
		_camera->x = (float)(cos(camdt) * 100.0f);
		_camera->z = (float)(sin(camdt) * 100.0f);
		_camera->y = 15;

		gluLookAt(_camera->x, _camera->y, _camera->z, 
			  0, 0, 0,
			  0, 1, 0);
	}

	
	//applylights();

	if(FOG_ON) glDisable(GL_FOG);
	//Floor
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glCallList(myDL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(FOG_ON) glEnable(GL_FOG);

	SSController_Update(deltaTime);

	SSController_Draw();

#ifdef _DEBUG
	OGLWindow_setOrthographicProjection();
	glPushMatrix();

		glLoadIdentity();
		sprintf(infotext, "X:%.1f Y:%.1f Z:%.1f", _camera->x, _camera->y, _camera->z);
		renderBitmapString(30, 15, (void *) OGLFONT, infotext);
		sprintf(infotext, "Angle:%.2f Anglev:%.2f", _camera->angle, _camera->anglev);
		renderBitmapString(30, 25, (void *) OGLFONT, infotext);
		sprintf(infotext, "lx:%.2f ly:%.2f lz:%.2f", _camera->lx, _camera->ly, _camera->lz);
		renderBitmapString(30, 35, (void *) OGLFONT, infotext);
		sprintf(infotext, "SPEED:%.2f", _camera->speed);
		renderBitmapString(30, 45, (void *) OGLFONT, infotext);
	glPopMatrix();


	glPushMatrix();
		glLoadIdentity();
		sprintf(infotext, "Days passed: %f", (float)days);
		renderBitmapString(30, 50, (void *) OGLFONT, infotext);
	glPopMatrix();

	glPushMatrix();
		glLoadIdentity();
		sprintf(infotext, "Time Scale: %f", TimeScale);
		renderBitmapString(30, 60, (void *) OGLFONT, infotext);
	glPopMatrix();

	glPushMatrix();
		glLoadIdentity();
		sprintf(infotext, "Camera Mode: %s", manualCam ? "Manual" : "Automatic");
		renderBitmapString(30, 70, (void *) OGLFONT, infotext);
	glPopMatrix();

	glPushMatrix();
		glLoadIdentity();
		FPS_update(_fpsCounter, gameTime->glut_elapsed_time);
		renderBitmapString(MAX(OGLWINDOW.width - 100, 0), 15, (void *) OGLFONT, _fpsCounter->stringfps);
		
		sprintf(infotext,"GameTime->SinceLastDisplay_Seconds: %4.4f", gameTime->sinceLastDisplay_seconds);
		renderBitmapString(MAX(OGLWINDOW.width - 350, 0), 25, (void *) OGLFONT, infotext);
		glPopMatrix();
	OGLWindow_resetPerspectiveProjection();
#endif

	TwDraw();


	glutSwapBuffers();

	glFlush();
}







void pause(){
		printf("Press any key to continue . . .");
		scanf("%c");
}


int main(int argc, char** argv)
{
#if _DEBUG
	objectCreationTest();
	_fpsCounter = FPS_new();
	//pause();
#endif

	initGL(argc,argv);

/*
#ifdef _DEBUG
	testSolarSystemController();
#endif
*/
	// Inicializações
	init();
	initCamera();
	initLights();
	initGameTime();
	SSController_Initialize();
	
	// Ciclo infinito do GLUT
	glutMainLoop();

	return 0;
}

void destroyObjects(void){
	#if _DEBUG
		FPS_delete(_fpsCounter);
	#endif
	Camera_delete(_camera);
	SSController_Delete();
	GameTime_delete(gameTime);

	TwTerminate();
}



void initGL(int argc, char** argv){
	// Inicialização do GLUT
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
	
	glutGameModeString(OGLWindow_getGameModeString());
	if(glutGameModeGet(GLUT_GAME_MODE_POSSIBLE))
	{
		OGLWindow_initGameMode();
	}
	else // Cria Janela Normal
	{
		OGLWindow_initWindowMode();
	}



	// Registar funções de callback
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(pressNormalKeys);
	glutKeyboardUpFunc(releaseNormalKeys);
	glutSpecialFunc(pressSpecialKey);
	glutSpecialUpFunc(releaseSpecialKey);
	glutMotionFunc(mouseMove);

	glutMouseFunc(mouseClick);

	// - Directly redirect GLUT mouse button events to AntTweakBar
	//glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
}

void turnLinearFogOn(void){
	GLfloat fogColor[] = {0., 0., 0., 1.0}; // Nevoeiro cinzento
		// Activar o nevoeiro
	glEnable(GL_FOG);

	// Define a cor do nevoeiro
	glFogfv(GL_FOG_COLOR, fogColor);

	// Define nevoeiro linear
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 2.0f);
	glFogf(GL_FOG_END, 40.0f);
}

void turnExpFogOn(void){
	GLfloat fogColor[] = {0., 0., 0., 1.0}; // Nevoeiro cinzento
	// Activar o nevoeiro
	glEnable(GL_FOG);

	// Define a cor do nevoeiro
	glFogfv(GL_FOG_COLOR, fogColor);

	// Define nevoeiro exponencial (1 - e^(-0.04d))
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.1f);
}

void turnExp2FogOn(void){
	GLfloat fogColor[] = {0., 0., 0., 1.0}; // Nevoeiro cinzento
	// Activar o nevoeiro
	glEnable(GL_FOG);

	// Define a cor do nevoeiro
	glFogfv(GL_FOG_COLOR, fogColor);
	// Define nevoeiro exponencial ao quadrado (1 - e^(-(0.04d)^2))
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogf(GL_FOG_DENSITY, 0.04f);
}

GLuint LoadTexture(char* filename) {
	GLuint returnTex = SOIL_load_OGL_texture(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y);

	if(returnTex == 0)
		return 0;

	glBindTexture(GL_TEXTURE_2D, returnTex);

	return returnTex;
}

void init(void)
{
	

	// Define técnica de shading: GL_FLAT, GL_SMOOTH
	glShadeModel(GL_SMOOTH);
	
	glPolygonMode(GL_FRONT, GL_FILL); // GL_LINE, GL_POINT, GL_FILL

	// Activa o teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D); 
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); 
	TwInit(TW_OPENGL, NULL);

	mainBar = TwNewBar("Parameters");
	TwAddVarRW(mainBar, "Time Scale", TW_TYPE_FLOAT, &TimeScale,
		" group='Simulation' keyincr=U keydecr=I help='Simulation's time scale'");
	TwAddVarRW(mainBar, "Speed", TW_TYPE_FLOAT, &camSpeed,
		" group='Camera' keyincr=S keydecr=X help='Camera movement speed' ");
	TwAddVarRW(mainBar, "Manual", TW_TYPE_BOOL16, &manualCam,
		" group='Camera' keyincr=C help='Toogle camera mode' ");
	TwAddVarRW(mainBar, "Show Orbits", TW_TYPE_BOOL16, &orbits,
		" group='Orbits' keyincr=O help='Toogle orbits' ");

	// Compila o modelo
	createFloorToDL();
}




void initCamera(void)
{
	_camera = Camera_new_initialized();
}

void initGameTime(void){
	gameTime = GameTime_new();
	gameTime->glut_elapsed_time = 0;
	gameTime->sinceLastDisplay_milis = 0.0f;
	gameTime->sinceLastDisplay_seconds = 0.0f;
}

void initLights(void)
{
	// Define a luz ambiente global
	GLfloat global_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	// Define a luz light0. Existem 8 fontes de luz no total.
	GLfloat light0_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	GLfloat light0_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
	GLfloat light0_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// Define a luz light1. Existem 8 fontes de luz no total.
	GLfloat light1_ambient[] = {0.1f, 0.1f, 0.1f, 1.0f};
	GLfloat light1_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light1_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat spot_angle = 45.0f;
	GLfloat spot_exp = 12.0f; // Maior valor = maior concentração de luz no centro

	// Fonte de luz ambiente
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	// Fonte de luz posicional
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.05);

	// Fonte de luz cónica
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spot_exp);

	// Activa a utilização de iluminação
	glEnable(GL_LIGHTING);
	// Activa a fonte de luz light0
	glEnable(GL_LIGHT0);
	// Activa a fonte de luz light1
	glEnable(GL_LIGHT1);
}

void createFloorToDL(void)
{
	int x, z;

	myDL = glGenLists(1);

	glNewList(myDL,GL_COMPILE);
	// Floor
	for(x=-100; x<=100; x+=2)
	{
		for(z=-100; z<=100; z+=2)
		{
			applymaterial(4);
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glVertex3f(x, 0.0f, z);				// Top Left
				glVertex3f(x+1.0f, 0.0f, z);		// Top Right
				glVertex3f(x+1.0f, 0.0f, z-1.0f);	// Bottom Right
				glVertex3f(x, 0.0f, z-1.0f);		// Bottom Left
			glEnd();
		}
		for(z=-99; z<=100; z+=2)
		{
			applymaterial(4);
			glBegin(GL_QUADS);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(x, 0.0f, z);				// Top Left
				glVertex3f(x+1.0f, 0.0f, z);		// Top Right
				glVertex3f(x+1.0f, 0.0f, z-1.0f);	// Bottom Right
				glVertex3f(x, 0.0f, z-1.0f);		// Bottom Left
			glEnd();

		}
	}
	for(x=-99; x<=100; x+=2)
	{
		for(z=-99; z<=100; z+=2)
		{
			applymaterial(4);
		
			glBegin(GL_QUADS);
				glNormal3f(0.0, 1.0, 0.0);
				glVertex3f(x, 0.0f, z);				// Top Left
				glVertex3f(x+1.0f, 0.0f, z);		// Top Right
				glVertex3f(x+1.0f, 0.0f, z-1.0f);	// Bottom Right
				glVertex3f(x, 0.0f, z-1.0f);		// Bottom Left
			glEnd();
		}
		for(z=-100; z<=100; z+=2)
		{
			applymaterial(4);
			
			glBegin(GL_QUADS);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(x, 0.0f, z);				// Top Left
				glVertex3f(x+1.0f, 0.0f, z);		// Top Right
				glVertex3f(x+1.0f, 0.0f, z-1.0f);	// Bottom Right
				glVertex3f(x, 0.0f, z-1.0f);		// Bottom Left
			glEnd();
		}
	}
	glEndList();
}


void applylights(void)
{
	// Define a posição de light0
	GLfloat light0_position[] = {0.0f, 3.0f, 0.0f, 1.0f};
	// Define a posição de direcção de light1
	GLfloat spot_position[] = {0.0f, 3.0f, -10.0f, 1.0f};
	GLfloat spot_direction[] = {0.0f, -1.0f, 0.0f};

	// Aplica a light0
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	// Aplica a light1
	glLightfv(GL_LIGHT1, GL_POSITION, spot_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
	
	glDisable(GL_LIGHTING);

	// Desenha uma esfera que sinaliza a posição da light0
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0f, 3.0f, 0.0f);
		glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	// Desenha uma esfera que sinaliza a posição da light1
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0f, 3.0f, -10.0f);
		glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glEnable(GL_LIGHTING);
}



void applymaterial(int type)
{
	// Define as propriedades dos materiais
	// Type: 0 (Branco); 1 (Amarelo); 2 (Ciano); 3 (Branco-Emissor); 4 (cinza)
	GLfloat mat_ambient[5][4] = {{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {.7f, .7f, .7f, .7f}};
	GLfloat mat_diffuse[5][4] = {{0.5f, 0.5f, 0.5f, 1.0f}, {0.5f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.5f, 0.5f, 1.0f}, {0.5f, 0.5f, 0.5f, 1.0f}, {0.5f, 0.5f, 0.5f, 1.0f}};
	GLfloat mat_specular[5][4] = {{1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
	GLfloat mat_emission[5][4] = {{0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 0.0f}};
	GLfloat mat_shininess[5][1] = {{20.0f}, {20.0f}, {20.0f}, {20.0f},{20.0f} };

	if((type >= 0) && (type < 5))
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient[type]); // GL_FRONT, GL_FRONT_AND_BACK , GL_BACK, 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission[type]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess[type]);
	}
}



void renderBitmapString(float x, float y, void *font, char *string)
{
	char *c;
	
	glDisable(GL_LIGHTING);

	glColor3f(.0f, 1.0f, 0.0f);

	// set position to start drawing fonts
	glRasterPos2f(x, y);

	// loop all the characters in the string
	for(c=string; *c!='\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}

	glEnable(GL_LIGHTING);
}

void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(_camera->fov, (GLfloat) w / (GLfloat) h, _camera->zNear, _camera->zFar );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);

	TwWindowSize(w, h);
}


void pressNormalKeys(unsigned char key, int x, int y)
{
	if(key == 27) // ESC
	{
		destroyObjects();
		exit(0);

	}
	else if(key == '1')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	else if(key == '2')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if(key == '3')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if(key == '4'){
		turnLinearFogOn();
		FOG_ON = 1;
	}
	else if(key == '5'){
		turnExpFogOn();
		FOG_ON = 1;
	}
	else if(key == '6'){
		turnExp2FogOn();
		FOG_ON = 1;
	}
	else if(key == '7'){
		glDisable(GL_FOG);
		FOG_ON = 0;
	}
	else if((key == 'A') || (key == 'a'))
	{
		_camera->deltaAnglev = 0.02f;
	}
	else if((key == 'Z') || (key == 'z'))
	{
		_camera->deltaAnglev = -0.02f;
	}
	else if((key == 'S') || (key == 's'))
	{
		_camera->deltaSpeed = 0.01f;
	}
	else if((key == 'X') || (key == 'x'))
	{
		_camera->deltaSpeed = -0.01f;
	}
	else if((key == 'N') || (key == 'n'))
	{
		glutLeaveGameMode();
		
		OGLWindow_initWindowMode();
		
				// Inicializações
		init();
		initCamera();
		initLights();
		initGameTime();
		SSController_Initialize();

		// Registar funções de callback
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutIdleFunc(display);
		glutIgnoreKeyRepeat(1);
		glutKeyboardFunc(pressNormalKeys);
		glutKeyboardUpFunc(releaseNormalKeys);
		glutSpecialFunc(pressSpecialKey);
		glutSpecialUpFunc(releaseSpecialKey);
		glutMotionFunc(mouseMove);

	// - Directly redirect GLUT mouse button events to AntTweakBar
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	}
	else if((key == 'F') || (key == 'f'))
	{
		OGLWindow_initGameMode();
		
		// Inicializações
		init();
		initCamera();
		initLights();
		initGameTime();
		SSController_Initialize();

		// Registar funções de callback
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutIdleFunc(display);
		glutIgnoreKeyRepeat(1);
		glutKeyboardFunc(pressNormalKeys);
		glutKeyboardUpFunc(releaseNormalKeys);
		glutSpecialFunc(pressSpecialKey);
		glutSpecialUpFunc(releaseSpecialKey);
		glutMotionFunc(mouseMove);

	// - Directly redirect GLUT mouse button events to AntTweakBar
	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	// - Directly redirect GLUT mouse "passive" motion events to AntTweakBar (same as MouseMotion)
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);

	} else if(key=='U' || key=='u') {
		TimeScale -= 10.0f;
	} else if(key=='I' || key=='i') {
		TimeScale += 10.0f;
	} else if(key=='O' || key=='o') {
		Planet_setOrbitVisibility(!Planet_getOrbitVisibility());
		orbits = !orbits;
	} else if(key=='C' || key=='c') {
		manualCam = !manualCam;
	}
}


void releaseNormalKeys(unsigned char key, int x, int y)
{
	if((key == 'A') || (key == 'a'))
	{
		if(_camera->deltaAnglev > 0.0f) _camera->deltaAnglev = 0.0f;
	}
	else if((key == 'Z') || (key == 'z'))
	{
		if(_camera->deltaAnglev < 0.0f) _camera->deltaAnglev = 0.0f;
	}
	if((key == 'S') || (key == 's'))
	{
		if(_camera->deltaSpeed > 0.0f) _camera->deltaSpeed = 0.0f;
	}
	else if((key == 'X') || (key == 'x'))
	{
		if(_camera->deltaSpeed < 0.0f) _camera->deltaSpeed = 0.0f;
	}
}


void pressSpecialKey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
			_camera->deltaAngle = -0.02f;
		break;
		case GLUT_KEY_RIGHT:
			_camera->deltaAngle = 0.02f;
		break;
		case GLUT_KEY_UP:
			_camera->deltaMove = 1;
		break;
		case GLUT_KEY_DOWN:
			_camera->deltaMove = -1;
		break;
		default:
		break;
	}
}


void releaseSpecialKey(int key, int x, int y)
{
	int ret;
	switch (key)
	{
		case GLUT_KEY_LEFT:
			if(_camera->deltaAngle < 0.0f) _camera->deltaAngle = 0.0f;
		break;
		case GLUT_KEY_RIGHT:
			if(_camera->deltaAngle > 0.0f) _camera->deltaAngle = 0.0f;
		break;
		case GLUT_KEY_UP:
			if(_camera->deltaMove > 0.0f) _camera->deltaMove = 0.0f;
		break;
		case GLUT_KEY_DOWN:
			if(_camera->deltaMove < 0.0f) _camera->deltaMove = 0.0f;
		break;
		case GLUT_KEY_F2:
			ret = SOIL_save_screenshot("img.bmp",
				SOIL_SAVE_TYPE_BMP, 0,0,1344,768);
			if(ret)
				printf("\nScreensave saved!\n");
			else
				printf("\nScreensave NOT saved!");
		break;
		default:
		break;
	}
}

void mouseClick(int button, int state, int x, int y){
	int tw = TwEventMouseButtonGLUT(button,state,x,y);
	if(!tw){
		if(button == GLUT_LEFT_BUTTON) {
			if(state != GLUT_DOWN) {

				_camera->deltaAnglev= 0.0f; // this will prevent the not stoping camera bug
				_camera->deltaAngle = 0.0f;
			}
		}
	}
}

void mouseMove(int x, int y) {
	// update deltaAngle

	int twMove = TwMouseMotion(x, y);
	
	if(!twMove){
		int delta = lastX - x;
		int deltaY = lastY - y;

		//needs to be redone
		if(delta < -1)
		{
			_camera->deltaAngle = +0.02f;
		} else if(delta > 1) 
		{
			_camera->deltaAngle = -0.02f;
		} else _camera->deltaAngle = 0.0f;

		if(deltaY < -1)
		{
			_camera->deltaAnglev = -0.02f;
		} else if(deltaY > 1) 
		{
			_camera->deltaAnglev = +0.02f;
		} else _camera->deltaAnglev = 0.0f;
	

		lastX = x;
		lastY = y;
	}
}
