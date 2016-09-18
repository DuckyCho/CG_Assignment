// Assignment0_BounceRect.cpp : 튕기는 사각형들 과제
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <gl\glut.h>

#define RECTANGLE_COUNT 4

typedef struct BounceRect
{
	int rectIdx;
	GLfloat xPos;
	GLfloat yPos;
	GLfloat xStep;
	GLfloat yStep;
	GLfloat size;
	GLfloat color[3];
	BounceRect * innerRect;
} Br;

Br* rects;

GLfloat window_width;
GLfloat window_height;


void RenderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < RECTANGLE_COUNT; i++) {
		glColor3f(rects[i].color[0], rects[i].color[1], rects[i].color[2]);
		glRectf(rects[i].xPos, rects[i].yPos, rects[i].xPos + rects[i].size, rects[i].yPos + rects[i].size);
		glColor3f(rects[i].innerRect->color[0], rects[i].innerRect->color[1], rects[i].innerRect->color[2]);
		glRectf(rects[i].innerRect->xPos, rects[i].innerRect->yPos, rects[i].innerRect->xPos + rects[i].innerRect->size, rects[i].innerRect->yPos + rects[i].innerRect->size);
	}

	glutSwapBuffers();
}

void refreshRectPos(GLfloat * xPos, GLfloat * yPos, GLfloat * xStep, GLfloat * yStep, GLfloat * size, GLfloat windowWidth, GLfloat windowHeight) {
	if (*xPos > windowWidth - *size ||
		*xPos < -windowWidth)
		*xStep = -*xStep;

	if (*yPos > windowHeight - *size ||
		*yPos < -windowHeight)
		*yStep = -*yStep;

	if (*xPos > windowWidth - *size)
		*xPos = windowWidth - *size - 1;

	if (*yPos > windowHeight - *size)
		*yPos = windowHeight - *size - 1;

	*xPos += *xStep;
	*yPos += *yStep;
}

void refreshInnerRectPos(GLfloat * xPos, GLfloat * yPos, GLfloat * xStep, GLfloat * yStep, GLfloat * size, GLfloat minWidth, GLfloat maxWidth, GLfloat minHeight, GLfloat maxHeight, GLfloat * parentXstep, GLfloat * parentYstep) {

	*xPos += *parentXstep;
	*yPos += *parentYstep;

	if (*xPos > maxWidth - *size ||
		*xPos < minWidth)
		*xStep = -*xStep;

	if (*yPos > maxHeight - *size ||
		*yPos < minHeight)
		*yStep = -*yStep;

	if (*xPos > maxWidth - *size)
		*xPos = maxWidth - *size - 1;

	if (*yPos > maxHeight - *size)
		*yPos = maxHeight - *size - 1;

	*xPos += *xStep;
	*yPos += *yStep;
	
}

void checkCollision(int rectIdx) {
	Br targetRect = rects[rectIdx];
	for (int i = 0; i < RECTANGLE_COUNT; i++) {
		if (i == rectIdx) continue;

		Br checkRect = rects[i];

		bool collisionX = targetRect.xPos + targetRect.size >= checkRect.xPos &&
			checkRect.xPos + checkRect.size >= targetRect.xPos;
		bool collisionY = targetRect.yPos + targetRect.size >= checkRect.yPos &&
			checkRect.yPos + checkRect.size >= targetRect.yPos;

		if (collisionX && collisionY) {
			Br * leftRect = targetRect.xPos <= checkRect.xPos ? &rects[rectIdx] : &rects[i];
			Br * rightRect = leftRect->rectIdx == targetRect.rectIdx ? &rects[i] : &rects[rectIdx];
			Br * upRect = targetRect.yPos <= checkRect.yPos ? &rects[i] : &rects[rectIdx];
			Br * downRect = upRect->rectIdx == targetRect.rectIdx ? &rects[i] : &rects[rectIdx];

			leftRect->xStep = leftRect->xStep >= 0 ? -leftRect->xStep : leftRect->xStep;
			rightRect->xStep = rightRect->xStep <= 0 ? -rightRect->xStep : rightRect->xStep;
			upRect->yStep = upRect->yStep >= 0 ? upRect->yStep : -upRect->yStep;
			downRect->yStep = downRect->yStep <= 0 ? downRect->yStep : -downRect->yStep;
		}
	}

}

void TimerFunction(int value) {

	for (int i = 0; i < RECTANGLE_COUNT; i++) {
		refreshRectPos(&rects[i].xPos, &rects[i].yPos, &rects[i].xStep, &rects[i].yStep, &rects[i].size, window_width, window_height);
		refreshInnerRectPos( &rects[i].innerRect->xPos, &rects[i].innerRect->yPos, &rects[i].innerRect->xStep, &rects[i].innerRect->yStep, &rects[i].innerRect->size, rects[i].xPos, rects[i].xPos + rects[i].size, rects[i].yPos, rects[i].yPos+ rects[i].size, &rects[i].xStep, &rects[i].yStep);
		checkCollision(i);
	}
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

void SetupRC(void) {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ChangeSize(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h) {
		window_width = 100.0f;
		window_height = 100.0f*h / w;
		glOrtho(-100.0f, 100.0f, -window_height, window_height, 1.0, -1.0);
	}
	else {
		window_width = 100.0f*w / h;
		window_height = 100.0f;
		glOrtho(-window_width, window_width, -100.0f, 100.0f, 1.0, -1.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GenerateRandomRect() {
	srand(time(NULL));
	rects = (Br*)malloc(sizeof(Br) * RECTANGLE_COUNT);
	for (int i = 0; i < RECTANGLE_COUNT; i++) {

		/***** Struct Br ******/
		//  int rectIdx
		//  GLfloat xPos;
		//  GLfloat yPos;
		//  GLfloat xStep;
		//  GLfloat yStep;
		//  GLfloat size;
		//  GLfloat color[3];
		//  Br * innerRect;
		/**********************/

		int randomStepX = (rand() % 801) - 400;
		int randomStepY = (rand() % 801) - 400;
		float randomColorR = (float)(rand() % 255) / 255.0f;
		float randomColorG = (float)(rand() % 255) / 255.0f;
		float randomColorB = (float)(rand() % 255) / 255.0f;
		float randomSize = (float)(rand() % 51) + 25.0f;
		
		
		Br * inner = (Br*)malloc(sizeof(Br));		
		inner->rectIdx = i + RECTANGLE_COUNT;
		inner->xStep = ((rand() % 801) - 400) / 100 + 2.0f;
		inner->yStep = ((rand() % 801) - 400) / 100 + 2.0f;
		inner->size = randomSize * 0.6f;
		inner->xPos = 0.0f + ((randomSize - inner->size) / 2);
		inner->yPos = 0.0f + ((randomSize - inner->size) / 2);
		inner->color[0] = (float)(rand() % 255) / 255.0f;
		inner->color[1] = (float)(rand() % 255) / 255.0f;
		inner->color[2] = (float)(rand() % 255) / 255.0f;
		inner->innerRect = NULL;

		rects[i] = { i, 0.0f, 0.0f, (float)randomStepX / 50.0f, (float)randomStepY / 50.0f, randomSize,{ randomColorR, randomColorG, randomColorB }, inner };
	}
}

int main(void) {
	GenerateRandomRect();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Bounce Rectangle");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(2000, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
}