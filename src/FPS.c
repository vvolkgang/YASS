#include "FPS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl/glut.h>

// -------------
//
// Constructor
//
// -------------
FPS * FPS_new(void) {
	FPS * novo;

	if ((novo = ( FPS*)malloc(sizeof(FPS))) == NULL) { 
		exit(1);
	}
	memset(novo,0xda,sizeof(FPS));

	return novo;
}


// -------------
//
// Destructor
//
// -------------
void FPS_delete( FPS * fps) {
	

	memset(fps,0xdd,sizeof( FPS));
	free(fps);
}



void FPS_update(FPS * fps, int glut_elapsed_time)
{
	fps->frame++;

	fps->time = glutGet(GLUT_ELAPSED_TIME);

	if((fps->time - fps->timebase) > 1000)
	{
		sprintf(fps->stringfps,"FPS: %4.2f", fps->frame * 1000.0 / (fps->time - fps->timebase));
		fps->timebase = fps->time;
		fps->frame = 0;
	}
}