
// (C) Alvaro Salmador 2011
//  All rights reserved


#include "global_defs.h"

#include <list>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <deque>

using namespace std;

#include "NodeRoot.h"
#include "NodeBranch.h"
#include "NodeLeaf.h"
//#include "User.h"



NodeRoot::NodeRoot(int nlev) : Node(false,0) 
{
	assert(Root==NULL); // multiple roots = mal
	Root = this;

	if (--nlev<1) nlev=1;
	
	// generadores rapidos:
	// http://www.cs.umbc.edu/~squire/reference/polyhedra.shtml

/*OCTAEDRO
	float x[5] = { F_GEO_R,       0, -F_GEO_R,        0, F_GEO_R };
	float y[5] = {       0, F_GEO_R,        0, -F_GEO_R,       0 };

	for(int i=0; i<4; ++i)
	{
		vector3f	a(0.f, 0.f, F_GEO_R);
		vector3f	b(  x[i],   y[i], 0.f);
		vector3f	c(x[i+1], y[i+1], 0.f);

		float lon_adj = (float)LON_ADJ;
		// rotamos para cortar x el atlantico, no por el meridiano 0
		//b.x = b.x*cosf(-deg2rad(lon_adj)) + b.y*sinf(-deg2rad(lon_adj));
		//b.y = b.x*sinf(-deg2rad(lon_adj)) + b.y*cosf(-deg2rad(lon_adj));
		//c.x = c.x*cosf(-deg2rad(lon_adj)) + c.y*sinf(-deg2rad(lon_adj));
		//c.y = c.x*sinf(-deg2rad(lon_adj)) + c.y*cosf(-deg2rad(lon_adj));

		_branch[i] = new NodeBranch(a,b,c,nlev);
		_branch[i+4] = new NodeBranch(-a,c,b,nlev);
	}
*/

/* TETRAEDRO
vector3f w[4]; // 4 vertices with  x, y, z coordinate
  double Pi = 3.141592653589793238462643383279502884197;

  double phiaa  = -19.471220333; // the phi angle needed for generation 

  double r = D_GEO_R; //1.0; // any radius in which the polyhedron is inscribed 
  double phia = Pi*phiaa/180.0; // 1 set of three points 
  double the120 = Pi*120.0/180.0;
  w[0].x = 0.0;
  w[0].y = 0.0;
  w[0].z = r;
  double the = 0.0;
  for(int i=1; i<4; i++)
  {
    w[i].x=r*cos(the)*cos(phia);
    w[i].y=r*sin(the)*cos(phia);
    w[i].z=r*sin(phia);
    the = the+the120;
  }

	//  polygon(0,1,2);
  //polygon(0,2,3);
  //polygon(0,3,1);
  //polygon(1,2,3);
	_branch[0] = new NodeBranch(w[0],w[1],w[2],nlev);
	_branch[1] = new NodeBranch(w[0],w[2],w[3],nlev);
	_branch[2] = new NodeBranch(w[0],w[3],w[1],nlev);
	_branch[3] = new NodeBranch(w[1],w[2],w[3],nlev);
*/

  const double adjgeor = 1.258409; //ajuste para que sea circunscrito y no inscrito el icosaedro
	// we use double precision for polyhedron generation in this constructor
  vector3<double> w[12]; //  12 vertices with x, y, z coordinates 
  const double Pi = 3.141592653589793238462643383279502884197;
  int i;
  const double phiaa  = 26.56505; // phi needed for generation 
  const double r = D_GEO_R*adjgeor; //1.0; // any radius in which the polyhedron is inscribed 
  const double phia = Pi*phiaa/180.0; // 2 sets of four points 
  const double theb = Pi*36.0/180.0;  // offset second set 36 dgrees 
  const double the72 = Pi*72.0/180;   // step 72 degrees 
  w[0][0]=0.0;
  w[0][1]=0.0;
  w[0][2]= D_GEO_R*adjgeor; //r;
  w[11][0]=0.0;
  w[11][1]=0.0;
  w[11][2]= -D_GEO_R*adjgeor; //-r;
  double the = 0.0+ deg2rad(GEO_ADJLON);
  for(i=1; i<6; i++)
  {
    w[i][0]=r*cos(the)*cos(phia);
    w[i][1]=r*sin(the)*cos(phia);
    w[i][2]=r*sin(phia);
    the = the+the72;
  }
  the=theb+ deg2rad(GEO_ADJLON);
  for(i=6; i<11; i++)
  {
    w[i][0]=r*cos(the)*cos(-phia);
    w[i][1]=r*sin(the)*cos(-phia);
    w[i][2]=r*sin(-phia);
    the = the+the72;
  }

  const int vert[20][3] = { {0,1,2},  {0,2,3}, {0,3,4}, {0,4,5}, {0,5,1}, // up5
						    {11,7,6}, {11,8,7}, {11,9,8}, {11,10,9}, {11,6,10}, //dn5 
							{1,6,2}, {2,7,3}, {3,8,4}, {4,9,5}, {5,10,1},
						    {6,7,2}, {7,8,3}, {8,9,4}, {9,10,5}, {10,6,1}};

  // ^he reordenado vertices de dn5 y la mitad de los intermedios, para que los vec normales apunten igual todos

	for(i=0; i<20; ++i)
	{
		for(int m=0; m<3; ++m) //build neighbour list (_neighbour[triangle][edge]=neighbouring triangle)
		{
			int iv1=vert[i][m];
			int iv2=vert[i][(m+1)%3];
			_neighbour[i][m]=-1;

			// look for the neighbour: it's the one that shares the 2 verts for our triangle i edge m 
			for (int o=0; o<20; ++o) {
				if (o==i) continue;
				if ((vert[o][0]==iv1 || vert[o][1]==iv1 || vert[o][2]==iv1) &&
					(vert[o][0]==iv2 || vert[o][1]==iv2 || vert[o][2]==iv2)) {
					//assert(_neighbour[i][m]==-1);
					_neighbour[i][m] = o;
					break; //
				}
			}
			assert(_neighbour[i][m]>=0);
		}

		vector3f v1=w[vert[i][0]], v2=w[vert[i][1]], v3=w[vert[i][2]];
		_v1[i] = v1;
		_v2[i] = v2;
		_v3[i] = v3;

		//printf("adj=%f", (3.f*F_GEO_R)/(v1+v2+v3).norm()); getchar();
		
		_normal[i]=(v2-v1)^(v3-v2);
		_normal[i].normalize();

		_ax[i] = v3-v2;
		_ay[i] = (v1-v2)^(v1-v3); 
		_ay[i].normalize();  
		_ay[i] ^= _ax[i];
		 
		//ay[i] era (v1-v2) - _ax[i]*((v1-v2)*_ax[i]);

		//printf("_ax[%d].norm()=%f\n", i, _ax[i].norm());
		//printf("_ay[%d].norm()=%f\n", i, _ay[i].norm());
		
		_ax[i].normalize();
		_ay[i].normalize();

		//assert(_ax[i]*_ay[i]<1.E-3);
		assert(_ax[i].norm2()>1.E-3);

		/*printf("i%d: ax2(%f,%f), ay2(%f,%f)\n",i,  vto2(i,_ax[i]).x,vto2(i,_ax[i]).y, vto2(i,_ay[i]).x, vto2(i,_ay[i]).y);
		printf("ax(%f,%f,%f), ay(%f,%f,%f)\n", _ax[i].x,_ax[i].y,_ax[i].z, _ay[i].x,_ay[i].y,_ay[i].z);
		printf("\n");*/
	
		_branch[i] = new NodeBranch(vto2(i,v1), vto2(i,v2), vto2(i,v3),
	#ifdef GEO_GLNODES
									v1, v2, v3,
	#endif
									i, nlev, 1);
	
		_pedge[i][0] = v1^v2;  // plane vector for this edge's corresponding frustum face
		_pedge[i][1] = v2^v3;
		_pedge[i][2] = v3^v1;

		_pedge[i][0].normalize();
		_pedge[i][1].normalize();
		_pedge[i][2].normalize();
	}

	for(int j=0; j<20; ++j) //build masks (faces in/outside octant)
	{
		vector3f w1 = w[vert[j][0]];
		vector3f w2 = w[vert[j][1]];
		vector3f w3 = w[vert[j][2]];

		if ((w1.x<=0.f || w1.y<=0.f || w1.z<=0.f) &&
			(w2.x<=0.f || w2.y<=0.f || w2.z<=0.f) &&
			(w3.x<=0.f || w3.y<=0.f || w3.z<=0.f))
			_b[0].unset(j);

		if ((w1.x>=0.f || w1.y<=0.f || w1.z<=0.f) &&
			(w2.x>=0.f || w2.y<=0.f || w2.z<=0.f) &&
			(w3.x>=0.f || w3.y<=0.f || w3.z<=0.f))
			_b[1].unset(j);

		if ((w1.x<=0.f || w1.y>=0.f || w1.z<=0.f) &&
			(w2.x<=0.f || w2.y>=0.f || w2.z<=0.f) &&
			(w3.x<=0.f || w3.y>=0.f || w3.z<=0.f))
			_b[2].unset(j);

		if ((w1.x>=0.f || w1.y>=0.f || w1.z<=0.f) &&
			(w2.x>=0.f || w2.y>=0.f || w2.z<=0.f) &&
			(w3.x>=0.f || w3.y>=0.f || w3.z<=0.f))
			_b[3].unset(j);

		if ((w1.x<=0.f || w1.y<=0.f || w1.z>=0.f) &&
			(w2.x<=0.f || w2.y<=0.f || w2.z>=0.f) &&
			(w3.x<=0.f || w3.y<=0.f || w3.z>=0.f))
			_b[4].unset(j);

		if ((w1.x>=0.f || w1.y<=0.f || w1.z>=0.f) &&
			(w2.x>=0.f || w2.y<=0.f || w2.z>=0.f) &&
			(w3.x>=0.f || w3.y<=0.f || w3.z>=0.f))
			_b[5].unset(j);

		if ((w1.x<=0.f || w1.y>=0.f || w1.z>=0.f) &&
			(w2.x<=0.f || w2.y>=0.f || w2.z>=0.f) &&
			(w3.x<=0.f || w3.y>=0.f || w3.z>=0.f))
			_b[6].unset(j);

		if ((w1.x>=0.f || w1.y>=0.f || w1.z>=0.f) &&
			(w2.x>=0.f || w2.y>=0.f || w2.z>=0.f) &&
			(w3.x>=0.f || w3.y>=0.f || w3.z>=0.f))
			_b[7].unset(j);
	}
}


int NodeRoot::find(NodeVisitor* nv, float lat, float lon, float r)
{
	assert(lat>=-90.f && lat<=90.f && lon>=-180.f && lon<=180.f);
	int i=0;
	
	float dedge[3];
	const vector3f P = latlon2vector(lat,lon);
	lon = deg2rad(lon);
	lat = deg2rad(lat);
	r = rreal2rsphere(r);
	
	int j,k, e; 

#ifdef GEO_GLNODES
	_findP = P;
	_findR = (r<0.f) ? .5f : r;
#endif

	// discard top or bottom 5 faces -now redundant
	Bitmask	mask(lat<0.f ? 0x000FFFE0 : 0x000FFC1F);
	//discard faces which are entirely outside this point's octant
	mask &= _b[ ((P.x<0.f)?1:0) | ((P.y<0.f)?2:0) | ((P.z<0.f)?4:0)];
	
	int search[20];
	for(j=0,k=0; j<20; ++j) 
		if (mask[j])
			search[k++] = j;
	
	//printf(" <k=%d> ", k);

	search[k]=-1;
	assert(k>0); //k==0 can't happen
	
	//int profile_nchecks=0;

	if (k==1) 
	{
	#ifdef GEO_GLNODES
		_findP2 = vto3(search[0],vto2(search[0],P)); //FIXME
		_findi = search[0];
	#endif
		if (r<0.f) {
			return _branch[search[0]]->find(nv, vto2(search[0],P));
		} else {
			for(e=0; e<3; ++e)
				dedge[e] = P*_pedge[search[0]][e];
			return find_branch(search[0], nv, P, r, dedge);  //_branch[search[0]]->find(nv, vto2(search[0],P), r);
		}
	}
	else
	{
		for(j=0; j<k; ++j)   // check each remaining face, see if P is in the o-edges frustum
		{
			assert(search[j]>=0 && search[j]<20);
			//profile_nchecks++;
			// project on each edge's plane vector
			for(e=0; e<3; ++e)
				dedge[e] = P*_pedge[search[j]][e];
			if (dedge[0]>=0.f && dedge[1]>=0.f && dedge[2]>=0.f)
			{
				//printf(" <prof_nchecks=%d> ", profile_nchecks);
	#ifdef GEO_GLNODES
				_findP2 = vto3(search[j],vto2(search[j],P)); //FIXME
				_findi = search[j];
	#endif
				if (r<0.f)
					return _branch[search[j]]->find(nv, vto2(search[j],P));
				else
					return find_branch(search[j], nv, P, r, dedge); //_branch[search[j]]->find(nv, vto2(search[j],P), r);
			}
		}
	}

	for(j=19; j>=0; --j) // cuando falla, es en los centrales no en top5 ni down5, x eso empezamos x ellos
	{
		if (mask[j]) 
			continue; //este ya ha sido comprobado..
		//profile_nchecks++;
		// project on each edge's plane vector
		for(e=0; e<3; ++e)
			dedge[e] = P*_pedge[j][e];
		if (dedge[0]>=0.f && dedge[1]>=0.f && dedge[2]>=0.f)
		{
			//printf(" x<prof_nchecks=%d> ", profile_nchecks);
	#ifdef GEO_GLNODES
			_findP2 = vto3(j,vto2(j,P)); //FIXME
			_findi = j;
	#endif
			if (r<0.f)
				return _branch[j]->find(nv, vto2(j,P));
			else
				return find_branch(j, nv, P, r, dedge); //_branch[j]->find(nv, vto2(j,P), r);
		}
	}
	printf(" FATAL ERROR: assert failed in NodeRoot::find lat=%f lon=%f\n", rad2deg(lat), rad2deg(lon));
	assert(false); //this really can't happen
	
	return -1;
}


inline int NodeRoot::find_branch(int nbranch, NodeVisitor* nv, const vector3f& P, float r, const float* dedge)
{
	//return _branch[nbranch]->find(nv, vto2(nbranch,P), r);
	// hay que llamar a los dos branch vecinos + cercanos a los q r llegue q cruzar tambien
	// esto sirve bien para zonas normales donde puede haber 1 borde que cruzar
	// pero no serviria bien en zonas donde confluyen 5 triangulos claro..
	// por eso las zonas de interseccion de 5 tri estan en zonas deshabitadas como
	// polos, mares, desiertos... (en general)

	int bvisit[]={0,1,2};

	if (dedge[0]>dedge[1]) {
		bvisit[0]=1;
		bvisit[1]=0;
	}
	if (dedge[2]<dedge[bvisit[1]]) {
		bvisit[1]=2;
	}
	// el primero y segundo son los dos de menor distancia pero no estan ordenados

	int ret = _branch[nbranch]->find(nv, vto2(nbranch,P), r);

	for(int i=0; i<=1; ++i) {
		const int e = bvisit[i];
		if (dedge[e]<=r)
			ret += _branch[_neighbour[nbranch][e]]->find(nv, vto2(_neighbour[nbranch][e],P), r);
	}

	return ret;
}


int NodeRoot::findAll(NodeVisitor* nv)
{
	int sum=0;
	for(int j=0; j<NROOT; ++j)
		sum += _branch[j]->findAll(nv);
	return sum;
}


NodeRoot::~NodeRoot() 
{
	for(int i=0; i<NROOT; ++i)
		delete _branch[i];

	Root = NULL;
}


