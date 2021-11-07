#include "pch.h"
#include "B711205.h"


//우주선 그리기
void B711205::DrawSpaceship() {
	glBegin(GL_TRIANGLE_FAN); //좌측날개
	glNormal3f(0, 0, 1);
	GLfloat mat_color[] = { 0.4, 0.7, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_color);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(-0.5, -0.5, 0);

	glEnd();

	glBegin(GL_TRIANGLE_FAN); //우측날개
	glNormal3f(0, 0, 1);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0.5, -0.5, 0);

	glEnd();

	GLfloat mat_color1[] = { 1, 0, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_color1);

	glPushMatrix();
	glRotatef(15, 0, 1, 0);
	
	glBegin(GL_TRIANGLE_FAN); //몸체
	glNormal3f(0, 0, 1);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, -0.25, -0.25);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(15, 0, -1, 0);
	glBegin(GL_TRIANGLE_FAN); //몸체
	glNormal3f(0, 0, 1);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, -0.25, -0.25);

	glEnd();
	glPopMatrix();
}

//행성 그리기
void B711205::DrawSphere() {
	GLUquadric* quad[20];

	GLfloat sphere_color1[] = { 0, 1, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_color1);
	glPushMatrix();
	glTranslatef(10, 10, 4);
	quad[0] = gluNewQuadric();
	glColor3f(0, 1, 0);
	gluSphere(quad[0], 3, 100, 100);
	glPopMatrix();

	GLfloat sphere_color2[] = { 1, 0, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sphere_color2);
	glPushMatrix();
	glTranslatef(0,100, -1);
	quad[1] = gluNewQuadric();
	glColor3f(1, 0, 0);
	gluSphere(quad[1], 6, 100, 100);
	glPopMatrix();
}


//Lighting
void B711205::SetLight() {
	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0); //임의의 광원(Ambient)
	GLfloat amb_color[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb_color);
}
