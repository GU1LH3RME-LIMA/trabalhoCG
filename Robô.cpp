//#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"
#include <math.h>

//#define _CRT_SECURE_NO_WARNINGS
#define PI 3.141592654
using namespace std;

char* fFlamengo = (char*)"./tronco.bmp";
char* fMetal = (char*)"./metal.bmp";
char* fEsfera = (char*)"./metal.bmp";
char* fOlho = (char*)"./olho.bmp";
char* fRosto = (char*)"./metal.bmp";
char* fBraco = (char*)"./metal.bmp";
char* fDedo = (char*)"./metal.bmp";
char* fJean = (char*)"./metal.bmp";

GLuint _jean;
GLuint _dedo;
GLuint _metal;
GLuint _braco;
GLuint _rosto;
GLuint _olho;
GLuint _tronco;
GLuint _disco;
GLuint _esfera;
GLuint _boca;
GLUquadric* quadSphere;
GLUquadric* quadCylinder;

bool textureOn = true;
float centroZ = 0;
float centroY = 0;
float diameterCylinder = 0.3;
float diameterSphere = 0.4;
float sizeArm = 4.5;
float sizeKnee = 4.5;
float sizeForearm = 4.0;
float sizeLeg = 5.0;
float sizeHand = 2.0;
float sizeClampPart = 1.0;
float diameterBase = 2.0;
float heightBase = 0.5;
float torsoHeight = 12;
float diameterTorso = 4;
float diameterKneck = 0.8;
float heightKneck = 2;
float diameterHead = 4.5;
float heightHead = 2;
float diameterEye = 1.2;
float diameterNose = 1;
float heightNose = 1.5;
float diameterMouth = 0.4;
float heightMouth = diameterHead / 1.5;

float LLegArt = 180.0f;
float LkneeArt = 0.0f;
float RLegArt = 180.0f;
float RkneeArt = 0.0f;
float headArt = 0.0f;

float eyeDistance = 100.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 15.0;
float zoom = 50;
float horizon = 0;
float vertic = 0;

float LangleArmZ = -90.0;
float LangleArmY = 0.0;
//float LangleArmX = -90.0;
float LangleForearm = 0.0;
float LangleHand = 0.0;
float LangleClampZ = 0.0;
float LangleClampY = 0.0;
float LhandSpin = 90;

float RangleArmZ = 90.0;
float RangleArmY = 0.0;
//float RangleArmX = -90.0;
float RangleForearm = 0.0;
float RangleHand = 0.0;
float RangleClampZ = 0.0;
float RangleClampY = 0.0;
float RhandSpin = 90;

GLfloat lightposition[4] = { 0.0f, 30.0f, 0.0f, 0.0f };
GLfloat luzAmbiente[4] = { 0.3, 0.3, 0.3, 0.0 };
bool interupArm=false;
bool interupLegg=false;
bool stopDancing=false;


//defines light source (LIGHT0)
void initLighting(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materials will follow current color
	glEnable(GL_COLOR_MATERIAL);
}

//makes the image into a texture, and returns the id of the texture
GLuint loadTexture(char* filename) {
	GLuint textureId;

	RgbImage theTexMap(filename); //Image with texture

	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);	//Tell OpenGL which texture to edit
												//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,	//Always GL_TEXTURE_2D
		0,						//0 for now
		GL_RGB,					//Format OpenGL uses for image
		theTexMap.GetNumCols(),	//Width 
		theTexMap.GetNumRows(),	//Height
		0,						//The border of the image
		GL_RGB,					//GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,		//GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		theTexMap.ImageData());	//The actual pixel data
	return textureId; //Returns the id of the texture
}

void initRendering(void) {
	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();
	_tronco = loadTexture(fFlamengo);
	_disco = loadTexture(fMetal);
	_esfera = loadTexture(fEsfera);
	_olho = loadTexture(fOlho);
	_rosto = loadTexture(fRosto);
	_braco = loadTexture(fBraco);
	_dedo = loadTexture(fDedo);
	_jean = loadTexture(fJean);
}

void My_timer_func( int t )
{
	if(!stopDancing)return;
		if ((LangleArmZ <= -90 + 45) and !interupArm){
			LangleArmZ += 3;
			RangleArmZ += 3;
			if(!(LangleArmZ <= -90 + 45)) interupArm=true;
		}else if((LangleArmZ >= -180 + 45) and interupArm ){
			LangleArmZ -= 3;
			RangleArmZ -= 3;
			if(!(LangleArmZ >= -180 + 45)) interupArm=false; 
		}if ((LkneeArt < 90) and !interupLegg){
			 LkneeArt += 3;
			 RkneeArt -= 3;
			 if (!(LkneeArt < 90)) interupLegg=true;
		}else if(LkneeArt > 0 and interupLegg) {
			LkneeArt -= 3;
			RkneeArt += 3;
			if(!(LkneeArt > 0)) interupLegg=false;
		}
	glutPostRedisplay();
	glutTimerFunc(50, My_timer_func, t);

}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	case 'w': //Aumenta a visualizaçao angulo z
		if (viewAngleZ < 180) viewAngleZ += 3;
		glutPostRedisplay();
		break;
	case 's': //Diminui a visualizaçao angulo z
		if (viewAngleZ > 0) viewAngleZ -= 3;
		glutPostRedisplay();
		break;
	case 'd': //Diminui a visualizaçao angulo x
		if (viewAngleX > -180) viewAngleX -= 3;
		glutPostRedisplay();
		break;
	case 'a': //Aumenta a visualizaçao angulo x
		if (viewAngleX < 180) viewAngleX += 3;
		glutPostRedisplay();
		break;
	
	case '1': //Aumenta angulo do braço esquerdo z
		if (LangleArmZ <= -90 + 45)
			LangleArmZ += 3;
		//if (angleArmZ >= 180) angleArmZ = 0;
		glutPostRedisplay();
		break;
	case '2': //Diminui angulo do braço esquerdo z
		if (LangleArmZ >= -180 + 45)
			LangleArmZ -= 3;
		//if (angleArmZ <= 0) angleArmZ = 360;
		glutPostRedisplay();
		break;
	case '3': //Aumenta angulo do braço esquerdo y
		if (LangleArmY <= 90 - 15) {
			LangleArmY += 3;
			//angleArmX -= 3;
		}
		//if (angleArmY >= 180) angleArmY = 0;
		glutPostRedisplay();
		break;
	case '4': //Diminui angulo do braço esquerdo y
		if (LangleArmY >= 0) {
			LangleArmY -= 3;
			//angleArmX += 3;
		}
		//if (angleArmY <= 0) angleArmY = 180;
		glutPostRedisplay();
		break;
	case '5': //Aumenta deformidade do antebraço esquerdo
		if (LangleForearm < 90) LangleForearm += 3;
		glutPostRedisplay();
		break;
	case '6': //Diminui deformidade do antebraço esquerdo
		if (LangleForearm > 0) LangleForearm -= 3;
		glutPostRedisplay();
		break;
	case '7': //Gira a garra esquerda horario
		if (LhandSpin > -180) LhandSpin -= 3;
		glutPostRedisplay();
		break;
	case '8': //Gira a garra esquerda ante- horario
		if (LhandSpin < 180) LhandSpin += 3;
		glutPostRedisplay();
		break;
	case '9': //Abre a garra esquerda
		if (LangleClampY < 60) LangleClampY += 3;
		glutPostRedisplay();
		break;
	case '0': //Fecha garra esquerda
		if (LangleClampY > 0) LangleClampY -= 3;
		glutPostRedisplay();
		break;
	case 92: //Aumenta angulo do braço direita z
		if (RangleArmZ >= 90 - 45)
			RangleArmZ -= 3;
		//if (angleArmZ >= 180) angleArmZ = 0;
		glutPostRedisplay();
		break;
	case 'z': //Diminui angulo do braço direita z
		if (RangleArmZ <= 90 + 45)
			RangleArmZ += 3;
		glutPostRedisplay();
		break;
	case 'x': //Aumenta angulo do braço direita y
		if (RangleArmY <= 90 - 15) {
			RangleArmY += 3;
			//angleArmX -= 3;
		}
		//if (angleArmY >= 180) angleArmY = 0;
		glutPostRedisplay();
		break;
	case 'c': //Diminui angulo do braço direita y
		if (RangleArmY >= 0) {
			RangleArmY -= 3;
			//angleArmX += 3;
		}
		//if (angleArmY <= 0) angleArmY = 180;
		glutPostRedisplay();
		break;
	case 'v': //Aumenta deformidade do antebraço direita
		if (RangleForearm < 90) RangleForearm += 3;
		glutPostRedisplay();
		break;
	case 'b': //Diminui deformidade do antebraço direita
		if (RangleForearm > 0) RangleForearm -= 3;
		glutPostRedisplay();
		break;
	case 'n': //Gira a garra direita horario
		if (RhandSpin > -180) RhandSpin -= 3;
		glutPostRedisplay();
		break;
	case 'm': //Gira a garra direita ante-horario
		if (RhandSpin < 180) RhandSpin += 3;
		glutPostRedisplay();
		break;
	case ',': //Abre garra direita
		if (RangleClampY < 60) RangleClampY += 3;
		glutPostRedisplay();
		break;
	case '.': //Fecha garra direita
		if (RangleClampY > 0) RangleClampY -= 3;
		glutPostRedisplay();
		break;
	case 'y': //Levanta toda perna esquerda
		if (LLegArt < 180 + 75) LLegArt += 3;
		glutPostRedisplay();
		break;
	case 'u': //Abaixa toda perna esquerda
		if (LLegArt > 180) LLegArt -= 3;
		glutPostRedisplay();
		break;
	case 'i': //"Prepara o chute" esquerda
		if (LkneeArt < 90) LkneeArt += 3;
		glutPostRedisplay();
		break;
	case 'o': //"Chuta" se a perna estiver inclinada esquerda
		if (LkneeArt > 0) LkneeArt -= 3;
		glutPostRedisplay();
		break;
	case 'h': // Levanta toda perna direita
		if (RLegArt < 180 + 75) RLegArt += 3;
		glutPostRedisplay();
		break;
	case 'j': //Abaixa toda perna direita
		if (RLegArt > 180) RLegArt -= 3;
		glutPostRedisplay();
		break;
	case 'k': //Prepara o chute" direita
		if (RkneeArt < 90) RkneeArt += 3;
		glutPostRedisplay();
		break;
	case 'l': //"Chuta" se a perna estiver inclinada direita
		if (RkneeArt > 0) RkneeArt -= 3;
		glutPostRedisplay();
		break;
	case 'q': //Vira a cabeça horario
		if (headArt < 90) headArt += 3;
		glutPostRedisplay();
		break;
	case 'e': //Vira a cabeça ante-horario
		if (headArt > -90) headArt -= 3;
		glutPostRedisplay();
		break;
	case '=': // Aumenta o zoom
		if (zoom > 4) zoom -= 2;
		glutPostRedisplay();
		break;
	case '-': // DIminui o zoom
		if (zoom < 60) zoom += 2;
		glutPostRedisplay();
		break;
	case ';': //vira flamenguista
		fFlamengo=(char*)"./flamengo.bmp";
		_tronco = loadTexture(fFlamengo);
		_braco = loadTexture(fFlamengo);
		glutPostRedisplay();
		break;
	case 't':
		if(!stopDancing){
			stopDancing=true;
			glutTimerFunc( 1, My_timer_func, 1);
		}
		else{
			stopDancing=false;

		}
		break;

	}
}

void glutInput(int key, int x, int y){
	switch (key){
	case GLUT_KEY_UP: // Sobe
		if (centroZ < 50) centroZ += 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN: // Desce
		if (centroZ > -50) centroZ -= 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT: // Vai para esquerda
		if (centroY < 50) centroY += 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT: // Vai para direita
		if (centroY > -50) centroY -= 2;
		glutPostRedisplay();
		break;
	}
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(90.0, (float)w / (float)h, 40.0, 50.0);
}

void drawCylinder(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _disco);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCylinderBr(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _braco);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCylinderP(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _rosto);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCylinderD(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _dedo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCylinderB(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _boca);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCylinderT(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _tronco);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCylinderJ(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _jean);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCone(float diameter, float lenght) {
	if (textureOn) {
		//glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}


void drawConeD(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _dedo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}

void drawDisk(float diameterInner, float diameterOuter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _disco);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}


void drawDiskF(float diameterInner, float diameterOuter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _rosto);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawDiskB(float diameterInner, float diameterOuter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _boca);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawSphere(float diameter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _esfera);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void drawSphereO(float diameter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _olho);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}



void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, 1, 20, 200);
	//glScalef(upX, upX, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	eyeX = eyeDistance * cos(viewAngleZ * PI / 180) * cos(viewAngleX * PI / 180);
	eyeY = eyeDistance * cos(viewAngleZ * PI / 180) * sin(viewAngleX * PI / 180);
	eyeZ = eyeDistance * sin(viewAngleZ * PI / 180);
	if (viewAngleZ < 90)
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1);
	else
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1);

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// drawing color
	glColor3f(1.0f, 1.0f, 1.0f);

	glPushMatrix(); //Origem

	//drawSphereO(5 * diameterEye);

	//Perna esquerda
	glTranslatef(0.0f, centroY + 3.8f, centroZ);
	glTranslatef(0.0f, 0.0f, heightBase);
	glTranslatef(0.0f, 0.0f, sizeKnee + diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	drawSphere(1.5 * diameterSphere);
	glRotatef(LLegArt, 0.0f, -1.0f, 0.0f);

	//Coxa
	drawCylinderJ(1.5 * diameterCylinder, sizeLeg);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	glTranslatef(0.0f, 0.0f, (diameterSphere / 5));
	drawSphere(1.5 * diameterSphere);
	glRotatef(LkneeArt, 0.0f, 1.0f, 0.0f);
	drawCylinderJ(1.5 * diameterCylinder, sizeKnee);
	glTranslatef(0.0f, 0.0f, (sizeKnee + diameterSphere / 5));
	drawDisk(0, diameterBase);
	drawCylinder(diameterBase, heightBase);
	glTranslatef(0.0f, 0.0f, heightBase);
	drawDisk(0, diameterBase);

	glPopMatrix();
	glPushMatrix();

	//Perna direita
	glTranslatef(0.0f, centroY-3.8f, centroZ);
	glTranslatef(0.0f, 0.0f, heightBase);
	glTranslatef(0.0f, 0.0f, sizeKnee + diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	drawSphere(1.5 * diameterSphere);
	glRotatef(RLegArt, 0.0f, -1.0f, 0.0f);

	//Coxa
	drawCylinderJ(1.5 * diameterCylinder, sizeLeg);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	glTranslatef(0.0f, 0.0f, (diameterSphere / 5));
	drawSphere(1.5 * diameterSphere);
	glRotatef(RkneeArt, 0.0f, 1.0f, 0.0f);
	drawCylinderJ(1.5 * diameterCylinder, sizeKnee);
	glTranslatef(0.0f, 0.0f, (sizeKnee + diameterSphere / 5));
	drawDisk(0, diameterBase);
	drawCylinder(diameterBase, heightBase);
	glTranslatef(0.0f, 0.0f, heightBase);
	drawDisk(0, diameterBase);

	//Tronco
	glPopMatrix();
	glPushMatrix();		//Parte de baixo do tronco
	glTranslatef(0.0f, centroY, centroZ + sizeLeg + sizeKnee + heightBase + diameterSphere/2);
	drawDisk(0, diameterTorso);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	drawCylinderT(diameterTorso, torsoHeight);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, torsoHeight);
	drawDisk(diameterCylinder, diameterTorso);

	//Pescoço e cabeça
	glPushMatrix();		//Base do pescoço
	drawCylinderP(diameterKneck, heightKneck);
	glTranslatef(0.0f, 0.0f, diameterKneck);
	//drawDisk(diameterCylinder, diameterKneck);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(headArt, 1.0f, 0.0f, 0.0f);
	glTranslatef(-diameterHead, 0.0f, 0.0f);
	glTranslatef(0.0, 0.0f, -heightHead/2);
	drawDisk(0, diameterHead);
	drawCylinder(diameterHead, heightHead);
	glTranslatef(0.0f, 0.0f, heightHead);
	drawDiskF(0, diameterHead);

	//glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	
	

	//Olhos
	glPushMatrix();		//centro do rosto
	
	
	glTranslatef(-diameterHead/6 , diameterHead / 3, -diameterEye / 2);
	glRotatef(-86.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	drawSphereO(diameterEye);
	drawSphereO(diameterEye);
	glTranslatef(0.0f,0.0f, diameterHead / 1.5);
	drawSphereO(diameterEye);

	

	
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();

	//Braço esquerdo
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, diameterTorso, -torsoHeight/3);
	drawSphere(2*diameterSphere);

	//move referencial do braço
	glRotatef(LangleArmZ, 1.0f, 0.0f, 0.0f);
	glRotatef(LangleArmY, 0.0f, 1.0f, 0.0f);

	//desenha braço
	drawCylinderBr(diameterCylinder, sizeArm);

	//move referencial do antebraço
	glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5);
	glRotatef(LangleForearm, 0.0f, 1.0f, 0.0f);

	//desenha antebraço
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCylinder(diameterCylinder, sizeForearm);

	//move para o referencial da garra
	glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 5);
	glRotatef(LangleClampZ , 0.0f, 0.0f, 1.0f);

	//desenha a esfera da garra
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 2);
	glRotatef(LhandSpin, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	//desenha a parte de cima da garra
	glRotatef(LangleClampY + 60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();
	glPushMatrix();

	//desenha parte de baixo da garra
	glRotatef(-LangleClampY - 60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();

	//Braço direita
	glPopMatrix();
	//glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, -diameterTorso, -torsoHeight / 3);
	drawSphere(2 * diameterSphere);

	//move referencial do braço
	glRotatef(RangleArmZ, 1.0f, 0.0f, 0.0f);
	glRotatef(RangleArmY, 0.0f, 1.0f, 0.0f);

	//desenha braço
	drawCylinderBr(diameterCylinder, sizeArm);

	//move referencial do antebraço
	glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5);
	glRotatef(RangleForearm, 0.0f, 1.0f, 0.0f);

	//desenha antebraço
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCylinder(diameterCylinder, sizeForearm);

	//move para o referencial da garra
	glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 5);
	glRotatef(RangleClampZ, 0.0f, 0.0f, 1.0f);

	//desenha a esfera da garra
	drawSphere(diameterSphere);
	glTranslatef(0.0f, 0.0f, diameterSphere / 2);
	glRotatef(RhandSpin, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	//desenha a parte de cima da garra
	glRotatef(RangleClampY + 60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();
	glPushMatrix();

	//desenha parte de baixo da garra
	glRotatef(-RangleClampY - 60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinderD(diameterCylinder / 3, sizeClampPart);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	

	glPopMatrix();


	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}



int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Robô- Trabalho CG- Guilherme de Lima e Gonçalves e Alves");

	initLighting();
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(glutInput);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}
