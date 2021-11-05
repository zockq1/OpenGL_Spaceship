#include "pch.h"
#include "B711205.h"

void B711205::DrawSpaceship() {
	glBegin(GL_TRIANGLE_FAN); //좌측날개
	glNormal3f(0, 0, 1);
	GLfloat mat_color[] = { 0.4, 0.7, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_color);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(-0.5, 0, 0);

	glEnd();

	glBegin(GL_TRIANGLE_FAN); //우측날개
	glNormal3f(0, 0, 1);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0.5, 0, 0);

	glEnd();

	glPushMatrix();
	glRotatef(5, 0, 1, 0);
	
	glBegin(GL_TRIANGLE_FAN); //몸체
	glNormal3f(0, 0, 1);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, -0.25);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(5, 0, -1, 0);
	glBegin(GL_TRIANGLE_FAN); //몸체
	glNormal3f(0, 0, 1);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, -0.25);

	glEnd();
	glPopMatrix();
}

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

void B711205::DrawXYZ() {//축 그리기
	glDisable(GL_LIGHTING);//조명을 키면 xyz축 선분이 회색이 되어서 Light 껐음, 선분에는 Normal이 없어서 그런 것 같습니다.

	glBegin(GL_LINE_LOOP);//X축
	glColor3f(0, 1, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(-10, 0, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);//Y축
	glColor3f(1, 0, 0);
	glVertex3f(0, 10, 0);
	glVertex3f(0, -10, 0);
	glEnd();

	glBegin(GL_LINE_LOOP);//Z축
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 10);
	glVertex3f(0, 0, -10);
	glEnd();
}

void B711205::DrawCube() {//정육면체 그리기
	glBegin(GL_TRIANGLE_FAN); //+X축 방향 면
	glNormal3f(1, 0, 0);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);

	glEnd();

	glBegin(GL_TRIANGLE_FAN); //-X축 방향 면
	glNormal3f(-1, 0, 0);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);

	glEnd();

	glBegin(GL_TRIANGLE_FAN); //+Y축 방향 면
	glNormal3f(0, 1, 0);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(0.5, 0.5, -0.5);

	glEnd();

	glBegin(GL_TRIANGLE_FAN); //+Y축 방향 면
	glNormal3f(0, -1, 0);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);

	glEnd();


	glBegin(GL_TRIANGLE_FAN); //+Z축 방향 면
	glNormal3f(0, 0, 1);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0.5, 0.5, 0.5);
	glVertex3f(-0.5, 0.5, 0.5);
	glVertex3f(-0.5, -0.5, 0.5);
	glVertex3f(0.5, -0.5, 0.5);

	glEnd();

	glBegin(GL_TRIANGLE_FAN); //-Z축 방향 면
	glNormal3f(0, 0, -1);

	glColor3f(0.4, 0.7, 1);
	glVertex3f(0.5, 0.5, -0.5);
	glVertex3f(-0.5, 0.5, -0.5);
	glVertex3f(-0.5, -0.5, -0.5);
	glVertex3f(0.5, -0.5, -0.5);

	glEnd();
}

void B711205::SetLight() {
	glEnable(GL_LIGHTING);
	
	glEnable(GL_LIGHT0); //임의의 광원(Ambient)
	GLfloat amb_color[] = { 0.1, 0.1, 0.1, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb_color);


	//glEnable(GL_LIGHT1); //초기 위치 (3.0, 3.0, 6.0, 1.0)에서 시작되는 광원(Diffuse)
	//GLfloat dif_pos[] = { 3.0, 3.0, 6.0, 1.0 };
	//GLfloat dif_color[] = { 1.0, 1.0, 1.0, 1.0 };
	//glLightfv(GL_LIGHT1, GL_POSITION, dif_pos);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, dif_color);



	
	
}

void B711205::SetSpotLight() {
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT2); //Spot light 설정
	GLfloat spot_pos[] = { 1.0, 2.0, 3.0, 1.0 };
	GLfloat amb_color[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat dif_color[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat spot_direction[] = { -1.0, -2.0, -3.0, 1.0 };
	glLightfv(GL_LIGHT2, GL_AMBIENT, amb_color);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dif_color);
	glLightfv(GL_LIGHT2, GL_POSITION, spot_pos);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 10.0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);



	GLfloat mat_color[] = { 0.10, 0.50, 0.80, 1.0 }; //cube의 재질
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_color);

}