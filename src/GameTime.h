#ifndef GAMETIME_H
#define GAMETIME_H

typedef struct _gameTime GameTime;

struct _gameTime
{
	int sinceLastDisplay_milis, glut_elapsed_time;
	float sinceLastDisplay_seconds;
	
} ;

/********
 * Constructor
 ********/
GameTime * GameTime_new(void);

/********
 * Destructor
 ********/
void GameTime_delete(GameTime * gameTime);


void GameTime_update(GameTime * gameTime);

#endif