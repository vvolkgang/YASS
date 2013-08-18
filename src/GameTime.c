#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl/glut.h>

#include "GameTime.h"


// -------------
//
// Constructor
//
// -------------



GameTime * GameTime_new(void) {
	GameTime * novo;

	if ((novo = ( GameTime*)malloc(sizeof(GameTime))) == NULL) { 
		exit(1);
	}
	memset(novo,0xda,sizeof(GameTime));

	return novo;
}


// -------------
//
// Destructor
//
// -------------
void GameTime_delete( GameTime * gameTime) {
	

	memset(gameTime,0xdd,sizeof( GameTime));
	free(gameTime);
}



void GameTime_update(GameTime * gameTime)
{
	int current = glutGet(GLUT_ELAPSED_TIME);
	gameTime->sinceLastDisplay_milis =  current - gameTime->glut_elapsed_time;
	gameTime->sinceLastDisplay_seconds = gameTime->sinceLastDisplay_milis / 1000.0f;
	gameTime->glut_elapsed_time = current;

}