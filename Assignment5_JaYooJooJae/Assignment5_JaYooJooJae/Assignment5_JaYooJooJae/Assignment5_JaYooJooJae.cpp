// Assignment5_JaYooJooJae.cpp

// 마지막 자유주제 과제입니다.

/*********** 과제 설명 ***************/
// 이 과제는
// 1.  .obj 파일 읽기 
// 2.  읽은파일 자료구조로 메모리에 저장
// 3.  opengl을 이용해 화면에 그리기
// 요런 작업을 수행합니다.
// 모든 obj 파일은 대응하지 못했고, 정점위치, 정점노말, face index만 읽는 loader입니다.

/*********** 과제 작동 ***************/
// 처음에 화살표키 좌우중하나를 누르지않으면 주전자가 안떠요 ㅠㅠㅠ
// depth_test를 꺼두어서 이상하게 보입니다. depthtest를 켜면 주전자가 안보여요 ㅠㅠ왜그런지 잘 모르겠습니다.
// 키보드 상하좌우 화살표로 주전자가 돌아갑니다.(정확하게는 카메라 시점이 주전자주위를 돕니다.) 
// 키보드 f1, f2로 주전자 축소확대가 됩니다.


// 1학기동안 강의하시느라 고생많으셨습니다. 정말 감사드립니다.

#include "stdafx.h"
#include <GL/glut.h>
#include <Windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>


typedef struct vector3 {
	double x;
	double y;
	double z;
} Vector3;

typedef struct vector2 {
	double x;
	double y;
} Vector2;

typedef struct face {
	int idx1;
	int idx2;
	int idx3;
} Face;

float sizeParam = 10.0f;
float xPos = 0.0f;
float yPos = 0.0f;
float zPos = 0.0f;
float posRad = 0.0f;
const float  dist = 10.0f;
GLfloat lightPos[] = { 0.0f, 0.0f, zPos, 1.0f };
GLfloat lightDirection[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat specref[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };

Vector3 * vertices;
Vector3 * normals;
Vector2 * uvs;
Face * faces;
int vertexCount = 0;
int vertexNormalCount = 0;
int uvCount = 0;
int faceCount = 0;

void SetupRC() {
	//glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
	glEnable(GL_LIGHT0);


	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void TimerFunc(int value) {
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunc, 1);
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

void drawObj() {

	for (int i = 0; i < faceCount; i++)
	{
		
		glColor3f( (((i + 75) % 125) + 131) / 255.0f, (((i+150) % 125) + 131) / 255.0f, ((i % 125) + 131) / 255.0f);
		glBegin(GL_TRIANGLES);
		int idx_1 = faces[i].idx1 -1;
		int idx_2 = faces[i].idx2 -1;
		int idx_3 = faces[i].idx3 -1;

		glTexCoord2f(uvs[idx_1].x, uvs[idx_1].y);
		glNormal3f(normals[idx_1].x, normals[idx_1].y, normals[idx_1].z);
		glVertex3f(vertices[idx_1].x * sizeParam, vertices[idx_1].y * sizeParam, vertices[idx_1].z * sizeParam);

		glTexCoord2f(uvs[idx_2].x, uvs[idx_2].y);
		glNormal3f(normals[idx_2].x, normals[idx_2].y, normals[idx_2].z);
		glVertex3f(vertices[idx_2].x * sizeParam, vertices[idx_2].y * sizeParam, vertices[idx_2].z * sizeParam);

		glTexCoord2f(uvs[idx_3].x, uvs[idx_3].y);
		glNormal3f(normals[idx_3].x, normals[idx_3].y, normals[idx_3].z);
		glVertex3f(vertices[idx_3].x*sizeParam, vertices[idx_3].y * sizeParam, vertices[idx_3].z * sizeParam);
		glEnd();
	}

}

void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();
	gluLookAt(
		xPos, yPos, zPos,      
		0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f     
	);
	drawObj();
	glFlush();
	glutSwapBuffers();

}

void ControlKey(int key, int x, int y) {
	if (key == GLUT_KEY_UP)
		yPos += 5.0f;

	if (key == GLUT_KEY_DOWN)
		yPos -= 5.0f;

	if (key == GLUT_KEY_LEFT) {
		posRad -= 0.0523599f;
		xPos = cos(posRad) * dist;
		zPos = -sin(posRad) * dist;
	}
		

	if (key == GLUT_KEY_RIGHT) {
		posRad += 0.0523599f;
		xPos = cos(posRad) * dist;
		zPos = -sin(posRad) * dist;
	}

	if (key == GLUT_KEY_F1) {
		sizeParam -= 5.0f;
	}
	if (key == GLUT_KEY_F2) {
		sizeParam += 5.0f;
	}
		

	glutPostRedisplay();
}

using namespace std;
void loadMesh(char * filePath) {

	char * stringFrag;
	size_t len = 0;
	size_t read;
	
	

	int faceTypeCount = 2;
	ifstream inFile(filePath);
	string lineString;
	char * line;
	while (getline(inFile, lineString)) {
		line = new char[lineString.length() + 1];
		strcpy(line, lineString.c_str());
		

		stringFrag = strtok(line, " ");
		if (!stringFrag) {
			delete[] line;
			continue;
		}	
		if (strcmp(stringFrag, "v") == 0) {
			vertexCount++;
			delete[] line;
			continue;
		}
		else if (strcmp(stringFrag, "vn") == 0) {
			vertexNormalCount++;
			delete[] line;
			continue;
		}
		else if (strcmp(stringFrag, "vt") == 0) {
			uvCount++;
			delete[] line;
			continue;
		}
		else if (strcmp(stringFrag, "f") == 0) {
			faceCount++;
			delete[] line;
			continue;
		}
		else {
			if(!line)
				delete[] line;
			continue;
		}
	}

	vertices = (Vector3 *)malloc(sizeof(Vector3)*vertexCount);
	normals = (Vector3 *)malloc(sizeof(Vector3)*vertexNormalCount);
	uvs = (Vector2 *)malloc(sizeof(Vector2)*uvCount);
	faces = (Face *)malloc(sizeof(Face)*faceTypeCount*faceCount);

	int vertexIdx = 0;
	int normalIdx = 0;
	int uvIdx = 0;
	int faceIdx = 0;

	ifstream inFile2(filePath);

	while (getline(inFile2, lineString)) {
		line = new char[lineString.length() + 1];
		strcpy(line, lineString.c_str());
		stringFrag = strtok(line, " ");
		if (!stringFrag) {
			delete[] line;
			continue;
		}
		if (strcmp(stringFrag, "v") == 0) {
			stringFrag = strtok(NULL, " ");

			vertices[vertexIdx].x = atof(stringFrag);
			stringFrag = strtok(NULL, " ");

			vertices[vertexIdx].y = atof(stringFrag);
			stringFrag = strtok(NULL, " ");

			vertices[vertexIdx].z = atof(stringFrag);
			vertexIdx++;
			
			delete[] line;
			continue;

		}

		else if (strcmp(stringFrag, "vn") == 0) {
			stringFrag = strtok(NULL, " ");
			normals[normalIdx].x = atof(stringFrag);
			stringFrag = strtok(NULL, " ");
			normals[normalIdx].y = atof(stringFrag);
			stringFrag = strtok(NULL, " ");
			normals[normalIdx].z = atof(stringFrag);
			normalIdx++;
			delete[] line;
			continue;
		}

		else if (strcmp(stringFrag, "vt") == 0) {
			stringFrag = strtok(NULL, " ");
			uvs[uvIdx].x = atof(stringFrag);
			stringFrag = strtok(NULL, " ");
			uvs[uvIdx].y = atof(stringFrag);
			printf("%lf %lf \n", uvs[uvIdx].x, uvs[uvIdx].y);
			uvIdx++;
			delete[] line;
			continue;
		}

		else if (strcmp(stringFrag, "f") == 0) {
			stringFrag = strtok(NULL, " ");
			faces[faceIdx].idx1 = atoi(stringFrag);
			stringFrag = strtok(NULL, " ");
			faces[faceIdx].idx2 = atoi(stringFrag);
			stringFrag = strtok(NULL, " ");
			faces[faceIdx].idx3 = atoi(stringFrag);
			faceIdx++;
			delete[] line;
			continue;
		}
		else {
			delete[] line;
			continue;
		}
	}
}



int main()
{
	loadMesh("./teapot.obj");
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Load Obj");
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunc, 1);
	glutSpecialFunc(ControlKey);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
}
