
// (C) Alvaro Salmador 2011
//  All rights reserved

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <GL/gl.h>
#include "glut.h"

GLuint texture[1];

double angle = 0;

typedef struct 
{
	int X;
	int Y;
	int Z;

	double U;
	double V;
} VERTICES;


const double PI = 3.1415926535897;
const int space = 10;
const int VertexCount = (91 / space) * (361 / space) * 4;

double dots[3000][3];
int dotofs=0;
VERTICES VERTEX[VertexCount];
GLuint LoadTextureRAW( const char * filename );

inline double deg2rad(double d)
{
	return d*PI/180.;
}

FILE *dlog=NULL;
void read_dots(FILE* f)
{
	if (!f) return;

	for(int i=dotofs; i<dotofs+1000; ++i)
	{
		char buf[100];
		float lat,lon;
		fgets(buf, sizeof(buf)-1, f);
		if (sscanf(buf, "%f,%f", &lon, &lat)==2) 
		{
			double R = 69.51;
			//lon+=90.;
			//if (lon>180.) lon-=360.;
			double Rp = R*cos(deg2rad(lat));
			dots[i%3000][2] = -R*sin(deg2rad(lat));
			dots[i%3000][0] = -Rp*sin(deg2rad(lon));
			dots[i%3000][1] = -Rp*cos(deg2rad(lon));
		}
		else
			i--;
		
		if (feof(f))
			fseek(f, 0, SEEK_SET);
	}

	dotofs+=1000;
	dotofs%=3000;
}


void DisplaySphere (double R, GLuint texture)
{
	int b;
	    
	glScalef (0.0125 * R, 0.0125 * R, 0.0125 * R);
	glRotatef (90., 1., 0., 0.);

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor3f(1.f,0.f,0.f);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., -100., 0.);
	glColor3f(0.f,1.f,0.f);
	glVertex3f(0., 0., 0.);
	glVertex3f(-100., 0., 0.);
	glColor3f(0.f,0.f,1.f);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., -100.);
	glColor3f(1.f,1.f,1.f);
	glEnd();

	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	int i=dotofs;
	glPointSize(45.f);
	for(; i<dotofs+1000; ++i)
		glVertex3dv(dots[i%3000]);
	glPointSize(15.f);
	for(; i<dotofs+2000; ++i)
		glVertex3dv(dots[i%3000]);
	glPointSize(30.f);
	for(; i<dotofs+3000; ++i)
		glVertex3dv(dots[i%3000]);
	glPointSize(1.f);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture);

	glBegin(GL_TRIANGLE_STRIP);
	for(b=0; b<VertexCount; b++)
	{
		glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
		glVertex3f(VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
	}

	b=VertexCount-1;;
	glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
	glVertex3f(VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);

	for (b=0; b<VertexCount; b++)
	{
		glTexCoord2f(VERTEX[b].U, -VERTEX[b].V);
		glVertex3f(VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);
	}
    
	glEnd();
}


void CreateSphere (double R, double H, double K, double Z) 
{
	int n=0;
	double a;
	double b;
	    
    
	for(b=0.; b<=90.-space; b+=space)
	{
		for(a=0.; a<=360.-space; a+=space)
		{
	            
			VERTEX[n].X = R * sin((a) / 180. * PI) * sin((b) / 180. * PI) - H;
			VERTEX[n].Y = R * cos((a) / 180. * PI) * sin((b) / 180. * PI) + K;
			VERTEX[n].Z = R * cos((b) / 180. * PI) - Z;
			VERTEX[n].V = (2 * b) / 360.;
			VERTEX[n].U = (a) / 360.;
			n++;
	
			VERTEX[n].X = R * sin((a) / 180. * PI) * sin((b + space) / 180. * PI) - H;
			VERTEX[n].Y = R * cos((a) / 180. * PI) * sin((b + space) / 180. * PI) + K;
			VERTEX[n].Z = R * cos((b + space) / 180 * PI) - Z;
			VERTEX[n].V = (2 * (b + space)) / 360;
			VERTEX[n].U = (a) / 360.;
			n++;

			VERTEX[n].X = R * sin((a + space) / 180. * PI) * sin((b) / 180. * PI) - H;
			VERTEX[n].Y = R * cos((a + space) / 180. * PI) * sin((b) / 180. * PI) + K;
			VERTEX[n].Z = R * cos((b) / 180. * PI) - Z;
			VERTEX[n].V = (2 * b) / 360.;
			VERTEX[n].U = (a + space) / 360.;
			n++;

			VERTEX[n].X = R * sin((a + space) / 180. * PI) * sin((b + space) /180. * PI) - H;
			VERTEX[n].Y = R * cos((a + space) / 180. * PI) * sin((b + space) / 180. * PI) + K;
			VERTEX[n].Z = R * cos((b + space) / 180. * PI) - Z;
			VERTEX[n].V = (2 * (b + space)) / 360.;
			VERTEX[n].U = (a + space) / 360.;
			n++;
            
		}

	}
}


void idle (void) 
{
	if (((int)angle)%3==0)
		read_dots(dlog);

	glutPostRedisplay();

	angle += 1.;
}

void display(void)
{
    glClearDepth(1);
    glClearColor(0.0, 0.0, 0.0, 1.0);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();

    glTranslatef(0.f, 0.f, -10.f);
    glRotatef(30., 1., 0., 0.);
    glRotatef(angle, 0., 1., 0.);
    
    DisplaySphere(5, texture[0]);
    
    glutSwapBuffers();
}



void init (void) 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	texture[0] = LoadTextureRAW("earth.raw");
	    
	CreateSphere(70., 0., 0., 0.);
}


void reshape (int w, int h) 
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	glMatrixMode (GL_MODELVIEW);
}



int main (int argc, char **argv) 
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(950, 650);
    glutInitWindowPosition(100, 50);
    
    glutCreateWindow("geolive");
    
	dlog = fopen("data.log", "rt");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutMainLoop();
    
    return 0;
}


GLuint LoadTextureRAW(const char * filename)
{
	GLuint texture;
	int width, height;
	unsigned char * data;

	FILE *f = fopen( filename, "rb" );
	if (!f)
		return 0;

	width = 1024;
	height = 512;
	data = (unsigned char *)malloc( width * height * 3 );

	fread(data, width * height * 3, 1, f);
	fclose(f);

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 	GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT );
	gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );

	free(data);

	return texture;

}