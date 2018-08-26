#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <Windows.h>
#include <string.h>


GLfloat TopLeftX, TopLeftY = 0;
GLfloat Width, Height;
int dwID;

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

#define BOOL int
#define TRUE 1
#define FALSE 0

GLfloat r[500];
GLfloat g[500];
GLfloat b[500];

GLfloat color[500];
BOOL a = FALSE;
GLubyte* LoadDIBitmap(const char *filename, BITMAPINFO **info)
{
	FILE *fp;
	GLubyte *bits;
	int bitsize, infosize;
	BITMAPFILEHEADER header;
	// 바이너리 읽기 모드로 파일을 연다
	if ((fp = fopen(filename, "rb")) == NULL)
		return NULL;
	// 비트맵 파일 헤더를 읽는다.
	if (fread(&header, sizeof(BITMAPFILEHEADER), 1, fp) < 1) {
		fclose(fp);
		return NULL;
	}
	// 파일이 BMP 파일인지 확인한다.
	if (header.bfType != 'MB') {
		fclose(fp);
		return NULL;
	}
	// BITMAPINFOHEADER 위치로 간다.
	infosize = header.bfOffBits - sizeof(BITMAPFILEHEADER);
	// 비트맵 이미지 데이터를 넣을 메모리 할당을 한다.
	if ((*info = (BITMAPINFO *)malloc(infosize)) == NULL) {
		fclose(fp);
		exit(0);
		return NULL;
	}
	// 비트맵 인포 헤더를 읽는다.
	if (fread(*info, 1, infosize, fp) < (unsigned int)infosize) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵의 크기 설정
	if ((bitsize = (*info)->bmiHeader.biSizeImage) == 0)
		bitsize = ((*info)->bmiHeader.biWidth *
		(*info)->bmiHeader.biBitCount + 7) / 8.0 *
		abs((*info)->bmiHeader.biHeight);
	// 비트맵의 크기만큼 메모리를 할당한다.
	if ((bits = (unsigned char *)malloc(bitsize)) == NULL) {
		free(*info);
		fclose(fp);
		return NULL;
	}
	// 비트맵 데이터를 bit(GLubyte 타입)에 저장한다.
	if (fread(bits, 1, bitsize, fp) < (unsigned int)bitsize) {
		free(*info); free(bits);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bits;
}

GLubyte* pBytes;
BITMAPINFO* info;
GLuint textures[6];

typedef float vec3_t[3];
enum {
	UP = 1,
	DOWN,
};

GLint oldX = -13;
GLint oldY = -13;
GLfloat backx1[100];
GLfloat backx2[100];

GLint mState = UP;
GLint cmode = 1;
vec3_t gRot = { 0,0,0 };

GLfloat Charrs = 0;
GLfloat Charrt = 0;

GLfloat rs[100];
GLfloat rs2[100];
GLfloat rs3[100];
GLfloat rt[100];

GLfloat sphereroll = 0.0f;
GLfloat radius = 2000;

GLfloat angle = 0.0f;
GLint checkcamera = -1;
GLint checkangle = 1;
GLfloat yangel = 0.0f;
GLfloat footangle = 0.0;
GLfloat footangle2 = 0.0;
GLfloat chickenmove = 0;
GLfloat chickenmovecheck = 1;

GLint soundswitch1 = 1;
GLint soundswitch2 = -1;
GLint soundswitch3 = 1;

GLint jumpcheck = 1;
GLfloat jump = 0.0;
GLfloat power = 3.0;
GLfloat jumpcount = 1;

GLfloat cx = 0.0f;
GLfloat cy = 0.0f;
GLfloat cz = 0.0f;

GLfloat snowObejct[3000][3];
GLfloat snowColor[3000][3];
GLfloat snowDrop[30000][3];

GLint test = 3;
GLint life = 3;
GLfloat level = 0.0f;

GLfloat collx = 30.0f;
GLfloat collx2 = -30.0f;
GLfloat takx[100];
GLfloat takx2[100];
GLint checkcoll = 1;
GLfloat chickangle = 240;
GLfloat stonex[30];


GLfloat ambientLight[] = { 0.15f, 0.15f, 0.15f, 1.0f };
GLfloat diffuseLight[] = { 0.15f, 0.15f, 0.0f, 1.0f };
GLfloat lightPos[] = { 0.0f, 1760, 1050, 1.0f };
GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat gray[] = { 0.75f, 0.75f, 0.75f };
GLfloat specref[] = { 0.2f, 0.2f, 0.2f, 1.0f };
time_t start, end;
GLdouble dif = 0.0f;

void setlight()
{
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor4f(0.75f, 0.75f, 0.75f, 1.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
	//glMateriali(GL_FRONT, GL_SHININESS, 10);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}
void setpos()
{
	for (int i = 0; i < 3000; ++i)
	{
		color[i] = (rand() % (10) + 1);
	}
	Charrs = 0;
	Charrt = 300;
	for (int i = 0; i < 500; ++i)
	{
		snowObejct[i][0] = (rand() % (800) - 400) * 10;//x값
		snowObejct[i][1] = (rand() % (1000) - 250) * 10;//y값
		snowObejct[i][2] = (rand() % (-360) + -500) * 10;//z값
	}
	for (int j = 0; j < 30; j++)
	{
		stonex[j] = (rand() % (360) + 0);
	}
	for (int i = 0; i < 100; i++)
	{
		rs2[i] = (rand() % (20) + 10);//15,-15
		rs3[i] = (rand() % (-10) - 20);//15,-15
		takx[i] = (rand() % (600) - 300);
		rt[i] = (rand() % (160) + 90);
		backx1[i] = (rand() % (60) + 45) * 10;
		backx2[i] = (rand() % (-38) - 50) * 10;
	}
}
GLint mode = 1;
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q')
	{
		exit(0);
	}
	if (key == 'r' || key == 'R')
	{
		soundswitch1 = 1;
		soundswitch2 = -1;
		soundswitch3 = 1;

		chickangle = 240;
		mode = 1;
		sphereroll = 0.0f;
		radius = 2000;
		angle = 0.0f;
		checkangle = 1;
		yangel = 0.0f;
		footangle = 0.0;
		footangle2 = 0.0;
		chickenmove = 0;
		chickenmovecheck = 1;
		checkcamera = -1;
		dif = 0.0f;
		jumpcheck = 1;
		jump = 0.0;
		power = 3.0;
		jumpcount = 1;
		cx = 0.0f;
		cy = 0.0f;
		cz = 0.0f;
		life = 3;
		collx = 30.0f;
		collx2 = -30.0f;
		checkcoll = 1;
		gRot[0] = 0;
		gRot[1] = 0;
		setpos();
		glutPostRedisplay();
	}

	if (key == 't' || key == 'T')//모드용
	{

		glutPostRedisplay();
	}
	if (key == 'c' || key == 'C')
	{
		checkcamera = checkcamera* -11;
		glutPostRedisplay();
	}
	if (key == 's' || key == 'S')
	{

		soundswitch1 = -1;
		soundswitch2 = 1;
		chickangle = 0;
		mode = mode*-1;
		time(&start);
		glutPostRedisplay();
	}
	if (key == 'a' || key == 'A')
	{

		if (yangel > -7 && life>0) //&& jumpcount>0)
		{
			yangel -= 0.7;
			Charrs += 0.7;
			collx -= 17;
			collx2 -= 17;
		}
		glutPostRedisplay();
	}
	if (key == 'd' || key == 'D')
	{
		if (yangel < 7 && life>0)//&& jumpcount>0)
		{
			Charrs -= 0.7;
			yangel += 0.7;
			collx += 17;
			collx2 += 17;
		}
		glutPostRedisplay();
	}
}

//마우스 시점 회전
void clamp(vec3_t v)
{
	for (int i = 0; i < 3; i++)
		if (v[i] > 360 || v[i] < -360)
			v[i] = 0.0f;
}
void glutMotion(int x, int y)
{
	if (cmode > 0)
	{

		if (mState == DOWN)
		{
			gRot[0] -= ((oldY - y) * 180.0f) / 200.0f;
			gRot[1] -= ((oldX - x) * 180.0f) / 200.0f;
			clamp(gRot);
			glutPostRedisplay();
		}
		oldX = x;
		oldY = y;
	}
}
void glutMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
		case GLUT_RIGHT_BUTTON:
			mState = DOWN;
			oldX = x;
			oldY = y;
			break;
		}
	}
	else if (state == GLUT_UP)
		mState = UP;
}

void star()
{
	for (int i = 0; i<500; ++i)
	{
		glPushMatrix();
		glColor3f(1, 1, 1);
		if (color[i] > 5)
		{
			glColor3f(r[i], g[i], b[i]);
		}
		glTranslatef(snowObejct[i][0], snowObejct[i][1], snowObejct[i][2]);//X,Y,Z위치
		glutSolidSphere(5, 10, 10);
		glPopMatrix();
	}
}
void rollingmytimer()
{
	if (mode < 0)
	{
		if (life > 0)
		{
			time(&end);
			dif = difftime(end, start);
		}
		if (checkcoll > 0)
		{
			for (int i = 0; i < 100; i++)
			{
				if (rt[i] < 330)
				{
					rt[i] += 0.5f;
				}
				if (rt[i] >= 330)
				{
					takx[i] = (rand() % (600) - 300);
					rt[i] = (rand() % (160) + 90);
				}
			}

		}

		for (int i = 0; i < 100; i++)
		{
			if ((Charrs*-30)<takx[i] + 20 && (Charrs*-30)>takx[i] - 15)
			{

				if (rt[i] == 311)
				{
					if (jumpcount > 0 && life>1)
					{
						jumpcheck = 4;
						checkangle = 0;
						angle = 0;
						footangle = 90.0f;
						footangle2 = -30.0f;
					}
					if (jumpcount > 0)
					{
						if (life > 0)
						{
							life -= 1;
						}
					}
					if (life < 1)
					{
						checkcoll = -1;
						mode = 1;

					}
				}
			}
		}
		if (checkangle > 0)
		{
			if (angle < 60)
			{
				angle += 60;
			}
			if (angle >= 60)
			{
				checkangle = checkangle*-1;
			}
		}
		if (checkangle < 0)
		{
			if (angle > -60)
			{
				angle -= 20;
			}
			if (angle <= -60)
			{
				checkangle = checkangle*-1;
			}
		}
		sphereroll += 0.1f;
	}

	glutPostRedisplay();
	glutTimerFunc(30, rollingmytimer, 1);
}
void settexture()
{
	glGenTextures(6, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//이미지 로딩을 한다. --- (2)
	pBytes = LoadDIBitmap("field.bmp", &info);
	//텍스처 설정 정의를 한다. --- (3)
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 398, 397, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
	//텍스처 파라미터 설정 --- (4)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_2D, textures[0]);
}
void mytimer()
{
	for (int i = 0; i < 3000; ++i)
	{
		r[i] = (rand() % (10) + 1) * 0.1;
		g[i] = (rand() % (10) + 1) *0.1;
	}
	if (mode < 0)
	{
		if (jumpcheck > 3)
		{
			jumpcount = -1;
			if (jump < 50)
			{
				jump += power;
			}
			if (jump >= 50)
			{
				jumpcheck = jumpcheck*-1;
			}
		}
		if (jumpcheck < 0)
		{
			if (jump > 0)
			{
				jump -= power;
			}
			if (jump <= 0)
			{
				jumpcheck = 1;
				checkangle = 1;
				jumpcount = 1;
				footangle = 0;
				footangle2 = 0;
			}
		}
	}
	if (chickenmovecheck > 0)
	{
		if (chickenmove > -3)
		{
			chickenmove -= 1;
		}
		if (chickenmove <= -3)
		{
			chickenmovecheck = chickenmovecheck*-1;
		}
	}
	if (chickenmovecheck < 0)
	{
		if (chickenmove < 0)
		{
			chickenmove += 1;
		}
		if (chickenmove >= 0)
		{
			chickenmovecheck = chickenmovecheck*-1;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(30, mytimer, 1);
}
void drawwtree()
{
	glPushMatrix();
	glColor3f(0.6, 0.4, 0);
	glTranslatef(0, 400, 0);
	glScalef(0.5, 1.0, 3.0);
	glutSolidCube(30);
	glTranslatef(0, 0, 10);
	glColor3f(0, 0.4, 0);
	glPushMatrix();
	glBegin(GL_QUADS);
	{

		glVertex3f(50.0, 10.0, 10.0f);
		glVertex3f(-50.0, 10.0, 10.0f);
		glVertex3f(-50.0, -10.0, 10.0f);
		glVertex3f(50.0, -10.0, 10.0f);
	}
	glEnd();
	glBegin(GL_QUADS);
	{

		glVertex3f(-50.0, 10.0, -10.0);
		glVertex3f(50.0, 10.0, -10.0);
		glVertex3f(50.0f, -10.0f, -10.0f);
		glVertex3f(-50.0, -10.0, -10.0);

	}
	glEnd();

	glBegin(GL_QUADS);
	{

		glVertex3f(-50.0, 10.0, 10.0f);
		glVertex3f(50.0, 10.0, 10.0f);
		glVertex3f(50.0, 10.0, -10.0);
		glVertex3f(-50.0, 10.0, -10.0);

	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glVertex3f(50.0f, -10.0f, -10.0f);
		glVertex3f(50.0, -10.0, 10.0f);
		glVertex3f(-50.0, -10.0, 10.0f);
		glVertex3f(-50.0, -10.0, -10.0);

	}
	glEnd();

	glBegin(GL_QUADS);
	{

		glVertex3f(-50.0, 10.0, 10.0f);
		glVertex3f(-50.0, 10.0, -10.0);
		glVertex3f(-50.0, -10.0, -10.0);
		glVertex3f(-50.0, -10.0, 10.0f);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glVertex3f(50.0, 10.0, -10.0);
		glVertex3f(50.0, 10.0, 10.0f);
		glVertex3f(50.0, -10.0, 10.0f);
		glVertex3f(50.0f, -10.0f, -10.0f);
	}
	glEnd();
	glPopMatrix();
	glPopMatrix();

}
void drawwtree2()
{
	glPushMatrix();
	glColor3f(0.6, 0.4, 0);
	glTranslatef(0, 400, 0);
	glScalef(0.5, 1.0, 3.0);
	glutSolidCube(30);
	glTranslatef(0, 0, 20);
	glScalef(70, 30, 10);
	glColor3f(0, 0.4, 0);
	glutSolidIcosahedron();
	glPopMatrix();
}
void drawwtree3()
{
	glPushMatrix();
	glColor3f(0.6, 0.4, 0);
	glTranslatef(0, 400, 0);
	glScalef(0.5, 1.0, 3.0);
	glutSolidCube(30);
	glTranslatef(0, 0, 10);
	glColor3f(0, 0.4, 0);
	glutSolidCone(50, 30, 10, 10);
	glPopMatrix();
}
void drawws3()
{
	glPushMatrix();
	glColor3f(0.6, 0.4, 0);
	glTranslatef(0, 300, 0);
	glScalef(0.5, 1.0, 3.0);
	glutSolidCube(30);
	glTranslatef(0, 0, 10);
	glColor3f(0, 0.4, 0);
	glutSolidCone(50, 30, 10, 10);
	glPopMatrix();
}

//
void drawback()
{
	glPushMatrix();
	glColor3f(0, 1, 0);
	glScalef(30, 30, 30);
	glutSolidIcosahedron();
	glPopMatrix();
}
void drawback2()
{
	glPushMatrix();
	glScalef(30, 30, 30);
	glutSolidIcosahedron();
	glPopMatrix();
}
//char model = 닭
void draw_char()
{
	//오른 발
	glPushMatrix();
	glTranslatef(-3, 0, 0);
	glTranslatef(0, -5, 0);
	glRotatef(-10, 0, 0, 1);
	glTranslatef(0, 0, 20);
	glTranslatef(0, -5, 0);
	glRotatef(-angle, 1, 0, 0);
	glTranslatef(0, 0, -20);

	glPushMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.2, 1.3, 0.2);
	glColor3f(1, 0.8, 0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-30, 0, 0, 1);
	glTranslatef(12, 0, 0);
	glTranslatef(0, 5, 0);
	glScalef(0.2, 1.3, 0.2);
	glColor3f(1, 0.8, 0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glTranslatef(5, 0, 0);
	glTranslatef(0, -5, 0);
	glScalef(0.2, 1.3, 0.2);
	glColor3f(1, 0.8, 0);
	glutSolidCube(5);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glColor3f(1, 0.8, 0);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glTranslatef(0, 0, 5);
	glColor3f(1, 0.8, 0);
	glScalef(0.3, 0.3, 3.0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glTranslatef(0, 0, 12);
	glColor3f(1, 1, 1);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1.3);
	glutSolidCube(8);
	glPopMatrix();

	glPopMatrix();

	//왼발
	glPushMatrix();
	glTranslatef(-20, 0, 0);
	glTranslatef(0, -5, 0);
	glTranslatef(0, -5, 0);
	glRotatef(10, 0, 0, 1);
	glTranslatef(0, 0, 20);
	glRotatef(angle, 1, 0, 0);
	glTranslatef(0, 0, -20);

	glPushMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glScalef(0.2, 1.3, 0.2);
	glColor3f(1, 0.8, 0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(-30, 0, 0, 1);
	glTranslatef(12, 0, 0);
	glTranslatef(0, 5, 0);
	glScalef(0.2, 1.3, 0.2);
	glColor3f(1, 0.8, 0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(30, 0, 0, 1);
	glTranslatef(5, 0, 0);
	glTranslatef(0, -5, 0);
	glScalef(0.2, 1.3, 0.2);
	glColor3f(1, 0.8, 0);
	glutSolidCube(5);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glColor3f(1, 0.8, 0);
	glutSolidSphere(1, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glTranslatef(0, 0, 5);
	glColor3f(1, 0.8, 0);
	glScalef(0.3, 0.3, 3.0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glTranslatef(0, 0, 12);
	glColor3f(1, 1, 1);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10, 0, 0);
	glTranslatef(0, -6, 0);
	glTranslatef(0, 0, 17);
	glColor3f(1, 1, 1.3);
	glutSolidCube(8);
	glPopMatrix();
	glPopMatrix();

	//몸통
	glPushMatrix();
	glTranslatef(0, 0, chickenmove);
	glPushMatrix();
	glTranslatef(0, -8, 0);
	glTranslatef(0, 0, 25);
	glTranslatef(-2, 0, 0);
	glColor3f(1, 1, 1.0);
	glScalef(1.0, 1.3, 1.0);
	glutSolidCube(15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -15, 0);
	glTranslatef(0, 0, 25);
	glTranslatef(-2, 0, 0);
	glColor3f(1, 1, 1.0);
	glScalef(1.0, 1.3, 1.0);
	glutSolidCube(15);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -8, 0);
	glTranslatef(0, 0, 35);
	glTranslatef(-2, 0, 0);
	glColor3f(1, 1, 1.0);
	glScalef(1.0, 1.3, 2.3);
	glutSolidCube(15);
	glPopMatrix();

	//오른날개
	glPushMatrix();
	glTranslatef(0, -15, 0);
	glTranslatef(0, 0, 30);
	glTranslatef(10.2, 0, 0);
	glTranslatef(-2, 0, 0);
	glRotatef(30, 0, 0, 1);
	glRotatef(footangle2, 0, 0, 1);
	glRotatef(footangle, 0, 1, 0);
	glColor3f(1, 1, 1.0);
	glScalef(0.3, 1.5, 1.5);
	glutSolidCube(15);
	glPopMatrix();

	//왼날개
	glPushMatrix();
	glTranslatef(0, -15, 0);
	glTranslatef(0, 0, 30);
	glTranslatef(-10.2, 0, 0);
	glTranslatef(-2, 0, 0);
	glRotatef(-30, 0, 0, 1);
	glRotatef(-footangle2, 0, 0, 1);
	glRotatef(footangle, 0, 1, 0);
	glColor3f(1, 1, 1.0);
	glScalef(0.3, 1.5, 1.5);
	glutSolidCube(15);
	glPopMatrix();

	//벼슬
	glPushMatrix();
	glTranslatef(0, -7, 0);
	glTranslatef(0, 0, 55);
	glTranslatef(-2, 0, 0);
	glColor3f(1, 0, 0.0);
	glScalef(0.3, 1.0, 0.8);
	glutSolidCube(15);
	glPopMatrix();

	//머리? 눈 부리
	glPushMatrix();
	//부리
	glTranslatef(0, -5, 0);
	glTranslatef(0, 0, 45);
	glTranslatef(-2, 0, 0);
	glColor3f(1, 0.8, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCone(5, 15, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 0);
	glTranslatef(0, 0, 40);
	glTranslatef(-2, 0, 0);
	glColor3f(1, 0, 0);
	glRotatef(-90, 1, 0, 0);
	glutSolidCube(5);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -5, 0);
	glTranslatef(0, 0, 45);
	glTranslatef(-10, 0, 0);
	glColor3f(0, 0, 0);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -5, 0);
	glTranslatef(0, 0, 45);
	glTranslatef(6, 0, 0);
	glColor3f(0, 0, 0);
	glutSolidSphere(2, 100, 100);
	glPopMatrix();

	glPopMatrix();
}
void drawworld()
{
	glPushMatrix();
	glRotatef(Charrs, 0, 0, 1);
	glRotatef(Charrt, 1, 0, 0);
	glTranslatef(0, 0, radius);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(1.2, 1.2, 1.2);
	glTranslatef(0, 0, jump);
	glScalef(0.7, 0.7, 0.7);
	glRotatef(chickangle, 0, 0, 1);
	draw_char();
	glPopMatrix();

	//장애물
	for (int i = 0; i < 30; ++i)
	{
		glPushMatrix();
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 1990);
		glColor3f(1, 0, 0);
		glTranslatef(takx[i], 0, 0);
		drawwtree3();
		glPopMatrix();
	}
	for (int i = 30; i < 60; ++i)
	{
		glPushMatrix();
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 1990);
		glColor3f(1, 0, 0);
		glTranslatef(takx[i], 0, 0);
		drawwtree2();
		glPopMatrix();
	}
	for (int i = 60; i < 80; ++i)
	{
		glPushMatrix();
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 1990);
		glColor3f(1, 0, 0);
		glTranslatef(takx[i], 0, 0);
		drawwtree();
		glPopMatrix();
	}
	for (int i = 80; i < 100; ++i)
	{
		glPushMatrix();
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 1990);
		glColor3f(0.75, 0.75, 0.75);
		glTranslatef(takx[i], 0, 0);
		drawws3();
		glPopMatrix();
	}

	//주변1
	for (int i = 0; i < 70; ++i)
	{
		glPushMatrix();
		glRotatef(rs2[i], 0, 0, 1);
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 2000);
		glColor3f(1, 0, 0);
		glScalef(1.2, 1.2, 1.2);
		drawback();
		glPopMatrix();
	}
	for (int i = 70; i < 100; ++i)
	{
		glPushMatrix();
		glRotatef(rs2[i], 0, 0, 1);
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 2010);
		glColor3f(0.75, 0.75, 0.75);
		glRotatef(stonex[i], 1, 1, 1);
		drawback2();
		glPopMatrix();
	}

	//주변2
	for (int i = 0; i < 70; ++i)
	{
		glPushMatrix();
		glRotatef(rs3[i], 0, 0, 1);
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 2000);
		glColor3f(1, 0, 0);
		glScalef(1.2, 1.2, 1.2);
		drawback();
		glPopMatrix();
	}
	for (int i = 70; i < 100; ++i)
	{
		glPushMatrix();
		glRotatef(rs3[i], 0, 0, 1);
		glRotatef(rt[i], 1, 0, 0);
		glTranslatef(0, 0, 2010);
		glColor3f(0.75, 0.75, 0.75);
		glRotatef(stonex[i], 1, 1, 1);
		drawback2();
		glPopMatrix();
	}

	//둥근세계

	glPushMatrix();
	//settexture();
	//glColor3f(1, 1, 1);
	glColor3f(0, 0.2, 0);
	glRotatef(sphereroll, 1.0, 0, 0);
	glutSolidSphere(radius, 250, 250);
	//glDisable(GL_TEXTURE_GEN_S);
	//glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();
}

// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.009f, 0.009f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glEnable(GL_CULL_FACE); // 뒷면에 대해서는 계산하지 말라
	glFrontFace(GL_CCW);   // 시계방향이 앞면이다.
	glShadeModel(GL_SMOOTH); // Enable Smooth Shading


	star();
	setlight();
	glPushMatrix();
	Reshape(800, 600);
	glPopMatrix();

	glLoadIdentity();
	if (checkcamera > 0)
	{
		glRotatef(gRot[0], 1.0, 0.0, 0.0);
		glRotatef(gRot[1], 0.0, 1.0, 0.0);
	}

	glPushMatrix();
	gluLookAt(0.0, 0.0, 90.0,
		0.0, 0.0, 0.0,
		0.0, 1.0, 0.0);
	glPopMatrix();

	glPushMatrix();
	drawworld();
	glPopMatrix();


	if (mode > 0 && life<1)
	{
		glPushMatrix();
		glColor3f(1, 1, 0);
		char string2[128];
		sprintf(string2, "press R -> Restart!", dif);
		if (checkcamera > 0)
		{
			glRasterPos3f(600, 3000, 500);
		}
		if (checkcamera < 0)
		{
			glRasterPos3f(-110, 2200, 500);
		}
		int len2 = (int)strlen(string2);
		for (int i = 0; i<len2; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string2[i]);
		}
		glPopMatrix();
		glPushMatrix();
		glColor3f(1, 1, 0);
		char string4[128];
		sprintf(string4, "Score = %.0f", dif);
		if (checkcamera > 0)
		{
			glRasterPos3f(600, 3000, 500);
		}
		if (checkcamera < 0)
		{
			glRasterPos3f(-50, 2400, 500);
		}
		int len4 = (int)strlen(string4);
		for (int i = 0; i<len4; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string4[i]);
		}
		glPopMatrix();
	}

	if (mode < 0)
	{
		glPushMatrix();
		glColor3f(1, 0, 0);
		char string[64];
		sprintf(string, "LIFE = %d", life);
		if (checkcamera > 0)
		{
			glRasterPos3f(-2500, 3000, 500);
		}
		if (checkcamera < 0)
		{
			glRasterPos3f(-400, 2400, 500);
		}
		int len = (int)strlen(string);
		for (int i = 0; i<len; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
		}
		glPopMatrix();

		glPushMatrix();
		glColor3f(1, 1, 0);
		char string2[128];
		sprintf(string2, "Score = %.0f", dif);
		if (checkcamera > 0)
		{
			glRasterPos3f(600, 3400, 500);
		}
		if (checkcamera < 0)
		{
			glRasterPos3f(-50, 2400, 500);
		}
		int len2 = (int)strlen(string2);
		for (int i = 0; i<len2; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string2[i]);
		}
		glPopMatrix();
	}
	if (mode > 0 && life>1)
	{
		glPushMatrix();
		glColor3f(1, 1, 0);
		char string3[128];
		sprintf(string3, "press S -> Start!", dif);
		if (checkcamera > 0)
		{
			glRasterPos3f(600, 3000, 500);
		}
		if (checkcamera < 0)
		{
			glRasterPos3f(-110, 2200, 500);
		}
		int len3 = (int)strlen(string3);
		for (int i = 0; i<len3; i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string3[i]);
		}
		glPopMatrix();
	}

	glutSwapBuffers();

}


GLvoid Reshape(int w, int h)
{
	Width = w;//화면의 가로 크기 얻어오기
	Height = h;//화면의 세로 크기 알아오기
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, w / h, 1.0, 8000.0);

	if (checkcamera < 0)
	{
		glTranslatef(0, 0, -500);
		glTranslatef(0, -2200, 0);
		glRotatef(-30, 1, 0, 0);
	}
	if (checkcamera > 0)
	{
		glTranslatef(0, -1500, 0);
		glTranslatef(0, 0, -5000);
	}

	glMatrixMode(GL_MODELVIEW);

}


void main(int argc, char *argv[])
{

	PlaySound(TEXT("title.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

	setpos();
	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // 디스플레이 모드 설정
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("jin_Termp"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glMatrixMode(GL_PROJECTION);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glLoadIdentity();
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(30, rollingmytimer, 1);
	glutTimerFunc(30, mytimer, 1);
	glutReshapeFunc(Reshape); // 다시 그리기 함수의 지정
	glutMainLoop();


	return 0;
}