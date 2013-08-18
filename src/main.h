#ifndef MAIN_H
#define MAIN_H



// Protótipos de funções
void display(void);
void reshape(GLsizei w, GLsizei h);
void pressNormalKeys(unsigned char key, int x, int y);
void releaseNormalKeys(unsigned char key, int x, int y);
void pressSpecialKey(int key, int x, int y);
void releaseSpecialKey(int key, int x, int y);
void getframerate(void);
void turnLinearFogOn(void);
void turnExpFogOn(void);
void turnExp2FogOn(void);
void renderBitmapString(float x, float y, void *font, char *string);

void createFloorToDL(void);

void init(void);
void initCamera(void);
void initLights(void);
void applylights(void);
void updateCameraPosition(void);
void applylights(void);
void applymaterial(int type);

void destroyObjects(void);
#endif