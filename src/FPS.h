#ifndef FPS_H
#define FPS_H

typedef struct _fps FPS;

struct _fps
{
	int frame, time, timebase;
	char stringfps[255];
} ;

/********
 * Constructor
 ********/
FPS * FPS_new(void);

/********
 * Destructor
 ********/
void FPS_delete(FPS * fps);


void FPS_update(FPS * fps, int glut_elapsed_time);

#endif