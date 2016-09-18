// Assignment1_Atom.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"

#include "lodepng.h"
#include <GL/glut.h>
#include <Windows.h>
#include <math.h>



#define GL_PI 3.1415f

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat zDistance = 0.0f;

GLfloat lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specref[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };

GLUquadricObj * Sun;
GLUquadricObj * Earth;
GLUquadricObj * Moon;

GLuint texID_Sun;
GLuint texID_Earth;
GLuint texID_Moon;

#define MODE_FLAT 1
#define MODE_SMOOTH 2
#define MODE_VERYLOW 3
#define MODE_MEDIUM 4
#define MODE_VERYHIGH 5
int iShade = MODE_FLAT;
int iTess = MODE_VERYLOW;

void ProcessMenu(int value) {
	switch (value)
	{
	case 1:
		iShade = MODE_FLAT;
		break;
	case 2:
		iShade = MODE_SMOOTH;
		break;
	case 3:
		iShade = MODE_VERYLOW;
		break;
	case 4:
		iShade = MODE_MEDIUM;
		break;
	case 5:
		iTess = MODE_VERYHIGH;
		break;
	default:
		iTess = MODE_VERYHIGH;
		break;
	}
	glutPostRedisplay();
}

void LoadTexture(char * fileName, GLuint * texID, GLUquadricObj ** obj) {
	std::vector<unsigned char> image;
	unsigned width, height, error;
	error = lodepng::decode(image, width, height, fileName);
	if (error)
		printf("Error!");
	
	glGenTextures(1, texID);
	glBindTexture(GL_TEXTURE_2D, *texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGBA, GL_UNSIGNED_BYTE,image.data());
		
	*obj = gluNewQuadric();
	gluQuadricDrawStyle(*obj, GLU_FILL);
	gluQuadricNormals(*obj, GLU_SMOOTH);
	gluQuadricOrientation(*obj, GLU_OUTSIDE);
	gluQuadricTexture(*obj, GL_TRUE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	
}


void SetupRC() {
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 64.0f);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
	glEnable(GL_LIGHT0);
	

	glEnable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TimerFunc(int value) {
	glutPostRedisplay();
	glutTimerFunc(100, TimerFunc, 1);
}

void ChangeSize(int w, int h) {
	GLfloat nRange = 100.0f;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange*2.0f, nRange*2.0f);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange*2.0f, nRange*2.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void RenderScene() {
	glShadeModel(GL_SMOOTH);

	static GLfloat fElect1 = 0.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texID_Sun);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluQuadricTexture(Sun, GLU_TRUE);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(Sun, 20, 16, 16);
	glPopMatrix();


	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glRotatef(fElect1, 0.0f, 1.0f, 0.0f);
	glTranslatef(60.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texID_Earth);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluQuadricTexture(Earth, GLU_TRUE);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(Earth, 14, 16, 16);
	

	glEnable(GL_TEXTURE_2D);
	glRotatef(fElect1 *24, 0.0f, 1.0f, 0.0f);
	glTranslatef(23.0f, 0.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, texID_Moon);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluQuadricTexture(Moon, GLU_TRUE);
	glColor3f(1.0f, 1.0f, 1.0f);
	gluSphere(Moon, 7, 16, 16);
	glPopMatrix();

	fElect1 += 1.0f;
	if (fElect1 > 360.0f)
		fElect1 = 0.0f;

	glutSwapBuffers();

}

void ControlKey(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;

	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;

	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;

	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;

	glutPostRedisplay();
}

int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Atom");
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunc, 1);
	glutSpecialFunc(ControlKey);
	glutDisplayFunc(RenderScene);
	SetupRC();
	LoadTexture("Sun.png", &texID_Sun, &Sun);
	LoadTexture("Earth.png", &texID_Earth, &Earth);
	LoadTexture("Moon.png", &texID_Moon, &Moon);
	glutMainLoop();
}

