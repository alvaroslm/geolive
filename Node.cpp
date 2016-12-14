
// (C) Alvaro Salmador 2011
//  All rights reserved

#include "global_defs.h"

#ifdef GEO_GLNODES
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <GL/gl.h>
#endif


#include "NodeRoot.h"
#include "NodeBranch.h"
#include "NodeLeaf.h"
//#include "User.h"


NodeRoot* Node::Root=NULL;


/// Drawing stuff for Node-derived classes:
#ifdef GEO_GLNODES

void NodeRoot::draw(int depth, const vector3f& eye) const
{
	//glLineWidth(1.0);
	GLfloat color0[4];
	glGetFloatv(GL_CURRENT_COLOR, color0);

	if (depth>=0)
		for(int i=0; i<NROOT; ++i) 
		{
			if (eye*_branch[i]->_normal>-0.1f)
			{
				glLineWidth(3.f);
			#if (VER_ICOSAEDRO>0)
				glColor3f(1.0f, 0.5f, 0.5f); //r
				draw_arc(_branch[i]->_pa3, _branch[i]->_pb3);
				glColor3f(0.5f, 1.0f, 0.5f); //g
				draw_arc(_branch[i]->_pb3, _branch[i]->_pc3);
				glColor3f(0.5f, 0.5f, 1.0f); //b
				draw_arc(_branch[i]->_pc3, _branch[i]->_pa3);
			#else //la unica diferencia en este #if son los colores
				if (i==0) glColor3f(0.5f, 0.5f, 1.0f);
				draw_arc(_branch[i]->_pa3, _branch[i]->_pb3);
				draw_arc(_branch[i]->_pb3, _branch[i]->_pc3);
				draw_arc(_branch[i]->_pc3, _branch[i]->_pa3);
				if (i==0) glColor3fv(color0);
			#endif

				/*glColor4f(1.0f, 1.0f, .3f, .2f); //ytrans
				glBegin(GL_TRIANGLES);
				{
					vector3f v=_branch[i]->_pa3;
					v*=GEO_GL_RAD/F_GEO_R*0.998f;
					glVertex3f(v.x,v.y,v.z);

					v=_branch[i]->_pb3;
					v*=GEO_GL_RAD/F_GEO_R;
					glVertex3f(v.x,v.y,v.z);

					v=_branch[i]->_pc3;
					v*=GEO_GL_RAD/F_GEO_R;
					glVertex3f(v.x,v.y,v.z);
				}
				glEnd();*/
	
				glColor3fv(color0);
			}
/*
   //esto muestra los vectores normales (del derecho salen mas, los invertidos queadn cerca
   //ademas los invertidos se ponen en rojo para distinguirlos bien (los otros en amarillo)
   // (usado para hacer q todos apuntasen hacia fuera)
			GLfloat color0[4];
			glGetFloatv(GL_CURRENT_COLOR, color0);
			glColor3f(1.f,1.f,0.f);
			vector3f v = _branch[i]->_normal; //(_branch[i]->_pb-_branch[i]->_pa) ^ (_branch[i]->_pc-_branch[i]->_pb);
			v.normalize();
			v *= GEO_GL_RAD/10.;
			vector3f w = (_branch[i]->_pa3+_branch[i]->_pb3+_branch[i]->_pc3)/3.f;
			w.normalize();
			w *= GEO_GL_RAD*1.12;
			v+=w;
			if (v.norm()<=GEO_GL_RAD*1.12)
					glColor3f(1.f,0.f,0.f);
			glBegin(GL_LINES);
			glVertex3f(0.f,0.f,0.f);
			glVertex3f(v.x, v.y, v.z);
			glEnd();
			glColor3fv(color0); /**/

			glLineWidth(1.f);
			if (depth>0)
				_branch[i]->draw(depth-1, eye);
		}

		if (_findR>=0.f) 
		{
			vector3f f=_findP;
			vector3f f2=_findP2;
			f.normalize(1.002f*GEO_GL_RAD); // ajustamos a la superficie de la esfera
			f2.normalize(1.002f*GEO_GL_RAD); // ajustamos a la superficie de la esfera

			glLineWidth(2.0f);
			glPointSize(5.f);

			glBegin(GL_POINTS);
			glColor3f(.9f, .7f, .8f);
			glVertex3f(f.x, f.y, f.z);

			glColor3f(0.65f, 0.65f, 1.f);
			glVertex3f(f2.x, f2.y, f2.z);
			glEnd();

			if (_findR>1.f) {
				glColor3f(0.65f, 0.65f, 1.f);
				draw_circ(_findP,_findR, _findi);
			}
			else {
				glColor3f(0.25f, 0.65f, 1.f);
				draw_circ(_findP, -1.f, _findi);
			}
		}
}

void NodeBranch::draw(int depth, const vector3f& eye) const
{
	float ecos = eye.cos(_normal);
	if (ecos<-.24+.22f/(1.f+depth)) //backface culling (-.2x for curvature)
		return;

	// dibujamos los hijos
	float color[3] = { 0.5f, 1.0f, 0.6f };
	/*float colorR[3] = { 1.0f, 0.5f, 0.6f };
	float* color;
	if (depth==0) color=colorR; else color=colorV;*/

	if (_u || _c) {
		if (_u && _u->_bColorMark || _c && _c->_bColorMark)
			draw_arc((_pa3+_pb3)/2.f, (_pa3+_pc3)/2.f, color, 2.5f);
		else
			draw_arc((_pa3+_pb3)/2.f, (_pa3+_pc3)/2.f);
	}
	if (_l || _c) {
		if (_l && _l->_bColorMark || _c && _c->_bColorMark)
			draw_arc((_pb3+_pc3)/2.f, (_pa3+_pb3)/2.f, color, 2.5f);
		else
			draw_arc((_pb3+_pc3)/2.f, (_pa3+_pb3)/2.f);
	}
	if (_r || _c) {
		if (_r && _r->_bColorMark || _c && _c->_bColorMark)
			draw_arc((_pb3+_pc3)/2.f, (_pa3+_pc3)/2.f, color, 2.5f);
		else
			draw_arc((_pb3+_pc3)/2.f, (_pa3+_pc3)/2.f);
	}

	int minlv = (ecos<0.75f) ?  // less detail near the horizon where lines bunch up (minlv=0 normally)
		(int)( (1.00f-ecos)/.25f ) : 0;

	if (depth>0 && minlv-_lvl<4)
	{
		if (_u) _u->draw(depth-1, eye);
		if (_l) _l->draw(depth-1, eye);
		if (_c) _c->draw(depth-1, eye);
		if (_r) _r->draw(depth-1, eye);
	}

}

void Node::draw_arc(vector3f a, vector3f b, float* color, float lwidth, int over) const
{
	float adjrad = 1.f+(0.001f*over);
	GLfloat linew0=1.f;
	GLfloat color0[4];
	glGetFloatv(GL_CURRENT_COLOR, color0);
	glGetFloatv(GL_LINE_WIDTH, &linew0);
	
	if (color) {
		glColor3fv(color); // en verde claro están los nodos x lo que ha pasado un find
		glLineWidth(lwidth);
	}

#if (VER_ICOSAEDRO>=1)
 #if (VER_ICOSAEDRO>=2)
  const NodeBranch* nb = dynamic_cast<const NodeBranch*>(this);
  if (!nb || nb->level()<=1)
 #endif
  {
	if (dynamic_cast<const NodeRoot*>(this))
	{
		// lo de este if es para ver el orden de las aristas (orientacion de la cara)
		// en los 20 nodos de root
		vector3f c=a^b;
		c.normalize(a.norm()*0.004f);
		a += c;
		c.normalize(b.norm()*0.004f);
		b += c;
		c = (b-a)*0.007f;
		b -= c;
		a += c;
	}

	a *= adjrad*GEO_GL_RAD/F_GEO_R;
	b *= adjrad*GEO_GL_RAD/F_GEO_R;

	glBegin(GL_LINES);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glEnd();
 #if (VER_ICOSAEDRO==1)
	glColor3fv(color0);
	glLineWidth(linew0);
	return;
 #endif
  }
#endif

	a.normalize(adjrad*GEO_GL_RAD); // ajustamos a la superficie de la esfera
	b.normalize(adjrad*GEO_GL_RAD); // (solo importa para dibujar bien)

	/*glPointSize(5.f);
	glBegin(GL_POINTS);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glEnd();*/

	float ang_total = a.ang(b);
	int nseg = (int)(ang_total*90.f/F_PI); // 1 segm por 2 grado max
		//(int)((b-a).norm()*60.f/GEO_GL_RAD); // 1 segm cada ~100km max 
	if (nseg<=1)
	{
	//a*=1.3;
	//b*=1.3;
		glBegin(GL_LINES);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
		glEnd();
		glColor3fv(color0);
		glLineWidth(linew0);
		return;
	}

	float ang_inc = ang_total/nseg;

	glBegin(GL_LINE_STRIP);
	glVertex3f(a.x, a.y, a.z);
	vector3f w = a^b^a;
	w.normalize(GEO_GL_RAD*adjrad);
	for(float ang=ang_inc; ang<ang_total; ang+=ang_inc)
	{
		vector3f v = a*cos(ang) + w*sin(ang);
		//v.normalize(GEO_GL_RAD*1.1);
		glVertex3f(v.x, v.y, v.z);
	}
	glVertex3f(b.x, b.y, b.z);
	glEnd();

	glColor3fv(color0);
	glLineWidth(linew0);
}


void NodeRoot::draw_circ(const vector3f& p, float r, int nface) const
{
	static float lastr=-1.f;
	
	if (r<0.f) {
		if (lastr<0.f) 
			return;
		else 
			r=lastr;
	}
	else lastr=r;

	vector2f o=vto2(nface,p);
	
	glBegin(GL_LINE_STRIP);
	for(int i=0; i<=36; ++i)
	{
		vector2f pt;
		float a=(F_PI/18.f)*(float)i;
		pt.x = o.x + r*cosf(a);
		pt.y = o.y + r*sinf(a);
		
		vector3f p3=vto3(nface, pt);
		p3.normalize(1.002f*GEO_GL_RAD); // ajustamos a la superficie de la esfera
		glVertex3f(p3.x, p3.y, p3.z);
	}
	glEnd();
}


void NodeLeaf::draw(int depth, const vector3f& eye) const
{
	if (_bColorMark) 
	{
		float color[3] = { 1.0f, 0.3f, 0.4f };
		draw_arc(vto3(_pa), vto3(_pb), color, 2.6f, 1);
		draw_arc(vto3(_pb), vto3(_pc), color, 2.6f, 1);
		draw_arc(vto3(_pc), vto3(_pa), color, 2.6f, 1);
	}
}

#endif
