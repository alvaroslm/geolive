
// (C) Alvaro Salmador 2011
//  All rights reserved

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <malloc.h>
#include <mmsystem.h> //timeGetTime
#include <math.h>
#include <GL/gl.h>
#include "glext.h" // <GL/glext.h>

#include <deque>
#include <list>
using namespace std;

#include "global_defs.h"
#include "NodeRoot.h"
#include "NodeVisitors.h"
#include "glut.h" //tiene q ir el ultimo


GLuint texture[1];

double angleX = 0.;
double angleY = 30.;
float zoom = 0.f;
vector3f Point;
float Radius=500E3f;

deque<pair<vector3f,vector3f> > listapuntos;

typedef struct 
{
	int X;
	int Y;
	int Z;

	double U;
	double V;
} VERTICES;


const int space = 10;
const int VertexCount = (91 / space) * (361 / space) * 4;

VERTICES VERTEX[VertexCount];
GLuint LoadTextureRAW(const char* filename, int width=0, int height=0);

NodeRoot* root;

FILE *dlog=NULL;


int read_dots(FILE* f, int ndots, float* data, bool eofexit=false)
{
	int i;
	if (!f) 
		return 0;
	
	for(i=0; i<ndots; ++i)
	{
		char buf[100];
		fgets(buf, sizeof(buf)-1, f);
	
		// el archivo tiene lon,lat, no lat,lon
		if (sscanf(buf, "%f,%f", &data[2*i+1], &data[2*i])!=2) 
			--i;

		if (feof(f)) {
			fseek(f, 0, SEEK_SET);
			if (eofexit) 
				return -i;
		}
	}

	return i;
}


void DisplaySphere(GLuint texture)
{
	int b;

	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, texture);

	glBegin(GL_TRIANGLE_STRIP);
	for(b=0; b<VertexCount; b++)
	{
		glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
		glVertex3f(-VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);
	}

	b=VertexCount-1;
	glTexCoord2f(VERTEX[b].U, VERTEX[b].V);
	glVertex3f(-VERTEX[b].X, VERTEX[b].Y, VERTEX[b].Z);

	for (b=0; b<VertexCount; b++)
	{
		glTexCoord2f(VERTEX[b].U, -VERTEX[b].V);
		glVertex3f(-VERTEX[b].X, VERTEX[b].Y, -VERTEX[b].Z);
	}
    
	glEnd();
}


void CreateSphere (double R, double H, double K, double Z) 
{
	int n=0;
	double a;
	double b;
	    
    //for(b=0.; b<=90.-space; b+=space)
	for(b=90.-space; b>=0.; b-=space)
	{
		//for(a=0.; a<=360.-space; a+=space)
		for(a=360.-space; a>=0; a-=space)
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
/*	if (((int)angle)%3==0)
		read_dots(dlog);*/

//	angle += 1.;
//	glutPostRedisplay();

	Sleep(20);
}


bool mousebutton1=false;
int xclick=0,yclick=0;
void mousebutton(int button, int state, int x, int y)
{
	if (button==GLUT_LEFT_BUTTON)
	{
		mousebutton1 = (state==GLUT_DOWN);
		xclick = x;
		yclick = y;
    }
}

void motion(int x, int y)
{
	if (mousebutton1)
	{
		float factor = (zoom>=0.f) ? 0.3f : (.3f/sqrtf(-zoom));
		angleX -= factor * (float)(x-xclick); // los ejes de pantalla salen del upper-left corner
		angleY += factor * (float)(y-yclick); // por eso el signo cambiado
				
		if (angleY>90.) angleY=90.;
		else if (angleY<-90.) angleY=-90.;
		
		if (angleX>180.) angleX-=360.;
		else if (angleX<-180.) angleX+=360.;
		
		xclick = x;
		yclick = y;

		MarkNodeVisitor unmarknode(false);
		MarkNodeVisitor marknode;
		root->findAll(&unmarknode);
		Point = latlon2vector(angleY, angleX);
		printf("marked: %d\n", root->find(&marknode, angleY, angleX)); /**/

		// busca todos los leaves cubiertos por P-r
		/*root->findAll(&unmarknode);
		Point = latlon2vector(angleY, angleX);
		printf("marked: %d\n", root->find(&marknode, angleY, angleX, Radius)); /**/

		glutPostRedisplay();
	}
}

void keyboard(unsigned char key, int x, int y)
{
	NullNodeVisitor nnv;

	switch(key)
	{
	case 27: //ESC
		printf("nodes (total): %d\n", obj_counter<Node>::N);
		printf("leaf nodes: %d\n", obj_counter<NodeLeaf>::N);
		delete root;
		printf("nodes (total) (should be zero): %d\n", obj_counter<Node>::N);
		printf("leaf nodes (should be zero): %d\n", obj_counter<NodeLeaf>::N);
		printf("sizeof(NodeBranch) = %d\n", sizeof(NodeBranch));
		printf("sizeof(NodeLeaf) = %d\n", sizeof(NodeLeaf));
		printf("sizeof(Msg) = %d\n", sizeof(Msg));
		printf("sizeof(User) = %d\n", sizeof(User));
		exit(0);
		break;

	case '+':
		zoom -= 1.f;
		glutPostRedisplay();
		break;

	case '-':
		zoom += 1.f;
		glutPostRedisplay();
		break;

	case 'r':
		if (Radius<100E3f)
			Radius=100E3f;
		else
			Radius+=100.E3f;
		goto casef;
	case 'v':
		Radius-=100E3f;
		if (Radius<100E3f)
			Radius=50E3f;
	case 'f': 
casef:
		{ // busca todos los leaves cubiertos por P-r
		MarkNodeVisitor unmarknode(false);
		MarkNodeVisitor marknode;
		root->findAll(&unmarknode); //printf(" unmarked: %d\n", root->findAll(&unmarknode));
		Point = latlon2vector(angleY, angleX);
		printf("marked: %d\n", root->find(&marknode, angleY, angleX, Radius));
		}
		glutPostRedisplay();
		break;

	case 't': //stress test de 'f'
		{
		  DWORD t0=timeGetTime();
		  DWORD timePassed=0;
		  int marked=0, i;
		  MarkNodeVisitor unmarknode(false);
		  MarkNodeVisitor marknode;

		  root->findAll(&unmarknode); //printf(" unmarked: %d\n", root->findAll(&unmarknode));

		  for(i=0; i<100000 && timePassed<500; ++i)
		  { // busca todos los leaves cubiertos por P-r
			Point = latlon2vector(angleY, angleX);
			marked = root->find(&marknode, angleY, angleX, Radius);
			timePassed = timeGetTime()-t0;
		  }

		  printf("marked: %d\n", marked);
		  printf("%d finds in %fs (%f finds/sec)\n", i, (float)timePassed/1000.f, (float)i*1000.f/(float)timePassed);
		}
		glutPostRedisplay();
		break;

	case 'p': {  // busca el nodo leaf q contiene a P
		MarkNodeVisitor unmarknode(false);
		MarkNodeVisitor marknode;
		root->findAll(&unmarknode); //printf(" unmarked: %d\n", root->findAll(&unmarknode));
		Point = latlon2vector(angleY, angleX);
		printf("marked: %d\n", root->find(&marknode, angleY, angleX));
		}
		glutPostRedisplay();
		break;

	case 'k': //test
		for(float lat=0.f; lat<45.f; lat+=0.5f)
			root->find(&nnv, lat, 0.f);
		for(float lon=0.f; lon<90.f; lon+=0.5f)
			root->find(&nnv, 0.f, lon);
		break;

	case 'l': {
		//point = latlon2vector(lat,lon);
		int n=0;
		float data[200*1000];
		int read=read_dots(dlog, 100*1000, data);
		for(int i=0; i<read; ++i)
			n += root->find(&nnv, data[2*i], data[2*i+1]);
		printf("inserted: %d\n", n);
		}
		glutPostRedisplay();
		break;

	case 'o': 
		//lee entero el file hasta el final
		int n=0;
		bool bExit=false;
		do {
			float data[200*1000];
			int read=read_dots(dlog, 100*1000, data, true);
			if (read<0) { bExit=true; read=-read; }
			for(int i=0; i<read; ++i)
				n += root->find(&nnv, data[2*i], data[2*i+1]);
		} while(!bExit);
		printf("inserted: %d\n", n);
		glutPostRedisplay();
		break;

	}
}


void display(void)
{
	static const GLfloat m[16] = { 1,0,0,0, 0,-1,0,0, 0,0,-1,0, 0,0,0,1 };
    glClearDepth(1);
    glClearColor(0.2, 0.2, 0.15, 1.0);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	//    glLoadIdentity(); //modelview
	glLoadMatrixf(m);

    glTranslatef(0.f, 0.f, 12.f+zoom);
    glRotatef(angleY, 1., 0., 0.); 
    glRotatef(angleX, 0., 1., 0.);
    
	glScalef (0.0125 * 5, 0.0125 * 5, 0.0125 * 5);
	glRotatef (90., 1., 0., 0.);
	glRotatef (-90., 0., 0., 1.);

	glColor4f(1.f,1.f,1.f,1.f);
	glPushMatrix();
	glRotatef(90., 0., 0., 1.);
	DisplaySphere(texture[0]);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor3f(1.f,0.f,0.f);
	glVertex3f(0., 0., 0.);
	glVertex3f(100., 0., 0.);
	glColor3f(0.f,1.f,0.f);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 100., 0.);
	glColor3f(0.f,0.f,1.f);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., 100.);
	glEnd();
	glColor4f(0.2f, 0.9f, 0.25f, 1.0f); //verde
	
	//float ax = -angleX+90.f; //test eye falso a 90º para ver culling de lineas
	//if (ax>180.f) ax-=360.f;
	
	vector3f eye = latlon2vector(angleY, angleX);
	root->draw(6, eye);

	if (!listapuntos.empty())
	{
		glPointSize(6.f);
		glBegin(GL_POINTS);
		while(!listapuntos.empty()) {
			vector3f v = listapuntos.begin()->first;
			const vector3f& c = listapuntos.begin()->second;
			v.normalize(GEO_GL_RAD*1.002f);
			glColor3f(c.x,c.y,c.z);
			glVertex3f(v.x,v.y,v.z);
			listapuntos.pop_front();
		}
		glEnd();
	}
	    
    glutSwapBuffers();
}



void init() 
{
	root = new NodeRoot(LEVEL_MIN);
	root->find(new MarkNodeVisitor(), 40.4, -3.7, 500.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	
	glDepthFunc(GL_LEQUAL);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW); //es el default de todas formas
	glEnable(GL_CULL_FACE);

	glEnable(GL_POINT_SMOOTH);
	//or GL_LINE_SMOOTH

	texture[0] = LoadTextureRAW("globcover_MOSAIC_H.raw"); // http://www.esa.int/esaEO/SEMGSY2IU7E_index_1.html
	//texture[0] = LoadTextureRAW("earth2048.raw", 2048, 1024); // earth-esa en 2048, carga mas rapido =)
	//texture[0] = LoadTextureRAW("earth-and-density.raw");
	//texture[0] = LoadTextureRAW("earth.raw");
	//texture[0] = LoadTextureRAW("earth-density.raw");
	//texture[0] = LoadTextureRAW("earth-esa.raw", 4096, 2048);
	//texture[0] = LoadTextureRAW("globcover_MOSAIC_H4096.raw", 4096, 2048); // http://www.esa.int/esaEO/SEMGSY2IU7E_index_1.html
	    
	CreateSphere(GEO_GL_RAD*1.008f, 0., 0., 0.);
}


void reshape (int w, int h) 
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(45., (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	glMatrixMode (GL_MODELVIEW);
}



int main(int argc, char **argv) 
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(950, 650);
    glutInitWindowPosition(100, 50);
    
    glutCreateWindow("geolive");
    
	dlog = fopen("data.log", "rt");

	printf("geolive (C) Alvaro Salmador 2011. All rights reserved\n");
	printf("Useful keys:\n Escape exits and prints some info\n");
	printf(" +/- zoom in/out\n");
	printf(" r/v increase/decrease search radius\n");
	printf(" f search inside view center+radius\n");
	printf(" t test find performance\n");
	printf(" p find leaf node in center\n");
	printf(" l read and insert some tweets from file\n");
	printf(" o read and insert ALL tweets from file\n");

    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);
    glutMainLoop();

    return 0;
}


GLuint LoadTextureRAW(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;

	FILE *f = fopen(filename, "rb");
	if (!f)
		return 0;

	if (width==0 || height==0) { // get from size, assuming width=2*height
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		height = sqrt(size/6.);
		width = 2*height;
		fseek(f, 0, SEEK_SET);
	}

	data = new unsigned char [width*height*3];

	fread(data, width*height*3, 1, f);
	fclose(f);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 	GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_REPEAT);

	float maxani=1.f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxani); // anisotropic filtering
	if (maxani>1.f)
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, (maxani>=8.f)?8.f:maxani);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	delete [] data;

	return texture;

}




