
// (C) Alvaro Salmador 2011
//  All rights reserved


#include "global_defs.h"

#include <math.h>
#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <set>
//#include <hashmap>
#include <deque>

using namespace std;

#include "Node.h"
#include "NodeBranch.h"
#include "NodeLeaf.h"
//#include "User.h"


////////////////////// branch:

NodeBranch::NodeBranch(const vector2f& a, const vector2f& b, const vector2f& c,
#ifdef GEO_GLNODES
					   const vector3f& a3, const vector3f& b3, const vector3f& c3,
#endif
					   char face, int nlev, int lvl)
	:	Node(false,face), _lvl(lvl),
#ifdef GEO_GLNODES
		_pa(a), _pb(b), _pc(c),
		_pa3(a3), _pb3(b3), _pc3(c3),
#endif
		_u(0), _l(0), _c(0), _r(0)
{
	//_p1,_p2,_p3 son los planos
	
	vector2f ab=(_pa+_pb)/2.f;
	vector2f ac=(_pa+_pc)/2.f;
	vector2f bc=(_pb+_pc)/2.f;

#ifdef GEO_GLNODES
	vector3f ab3=(_pa3+_pb3)/2.f;
	vector3f ac3=(_pa3+_pc3)/2.f;
	vector3f bc3=(_pb3+_pc3)/2.f;
#endif

	_p1 = (ac-ab).rotate_left();	// ab^ac;
	_p2 = (ab-bc).rotate_left();	// bc^ab;
	_p3 = (ac-bc).rotate_left();	// bc^ac;

/*if (lvl==1)
{
	printf("a(%f,%f), b(%f,%f)\n",a.x,a.y,  b.x, b.y);
	printf("c(%f,%f), p1(%f,%f)\n",c.x,c.y,  _p1.x, _p1.y);
	printf("p2(%f,%f), p3(%f,%f)\n",_p2.x,_p2.y,  _p3.x, _p3.y);
	printf("\n");
}*/

#ifdef GEO_GLNODES
	vector3f center = (_pa3+_pb3+_pc3)/3.f; 
	_cr = max( (center-_pa3).norm2(), (center-_pb3).norm2());
	center -= _pc3;
	_cr = max( _cr, center.norm2());
	_cr = sqrtf(_cr); // _cr es el radio de la circunferencia centrada que contiene al triangulo
#endif

	/*printf("NodeBranch::NodeBranch:\n");
	printf(" a(%.2f, %.2f, %.2f)\n", a.x, a.y, a.z);
	printf(" b(%.2f, %.2f, %.2f)\n", b.x, b.y, b.z);
	printf(" c(%.2f, %.2f, %.2f)\n", c.x, c.y, c.z);
	printf("\n\n");*/

	if (nlev>1)
	{
#ifdef GEO_GLNODES
		_u = new NodeBranch(_pa,  ab,  ac,  _pa3,  ab3,  ac3, face, nlev-1, lvl+1);
		_l = new NodeBranch( ab, _pb,  bc,   ab3, _pb3,  bc3, face, nlev-1, lvl+1);
		_c = new NodeBranch( bc,  ac,  ab,   bc3,  ac3,  ab3, face, nlev-1, lvl+1);
		_r = new NodeBranch( ac,  bc, _pc,   ac3,  bc3, _pc3, face, nlev-1, lvl+1);
#else
		_u = new NodeBranch(_pa,  ab,  ac, face, nlev-1, lvl+1);
		_l = new NodeBranch( ab, _pb,  bc, face, nlev-1, lvl+1);
		_c = new NodeBranch( bc,  ac,  ab, face, nlev-1, lvl+1);
		_r = new NodeBranch( ac,  bc, _pc, face, nlev-1, lvl+1);
#endif
	}
	else if (nlev==1)
	{
		/*_u = new NodeLeaf(this, _pa3,  ab3,  ac3);
		_l = new NodeLeaf(this,  ab3, _pb3,  bc3);
		_c = new NodeLeaf(this,  bc3,  ac3,  ab3);
		_r = new NodeLeaf(this,  ac3,  bc3, _pc3);*/
		/*_u = new NodeLeaf(this, _pa,  ab,  ac);
		_l = new NodeLeaf(this,  ab, _pb,  bc);
		_c = new NodeLeaf(this,  bc,  ac,  ab);
		_r = new NodeLeaf(this,  ac,  bc, _pc);*/
		_u = NULL;
		_l = NULL;
		_c = NULL;
		_r = NULL;
	}

#ifdef GEO_GLNODES
	vector3f na=_pa3, nb=_pb3, nc=_pc3;
	na.normalize();	// obtenemos v normal a la esfera en la cara proyectada sobre la esfera
	nb.normalize();	// ya que las caras que tenemos en cada branch estan en el mismo plano 
	nc.normalize(); // que la cara padre original del icosaedro. El v normal es necesrio al 
	_normal = (nb-na)^(nc-nb); // dibujar para bajar la calidad de las caras cerca del horizonte
#endif
}


Node* NodeBranch::create(char t) 
{
	vector2f ab=(_pa+_pb)/2.f;
	vector2f ac=(_pa+_pc)/2.f;
	vector2f bc=(_pb+_pc)/2.f;

	if (_lvl>=LEVEL_LEAF)
	{
		switch(t) {
		case 0: return new NodeLeaf(this, _pa,  ab,  ac);
		case 1: return new NodeLeaf(this,  ab, _pb,  bc);
		case 2: return new NodeLeaf(this,  bc,  ac,  ab);
		case 3: return new NodeLeaf(this,  ac,  bc, _pc);
		}
	}
	else
	{
	#ifdef GEO_GLNODES
		vector3f ab3=(_pa3+_pb3)/2.f;
		vector3f ac3=(_pa3+_pc3)/2.f;
		vector3f bc3=(_pb3+_pc3)/2.f;
		switch(t) {
		case 0: return new NodeBranch(_pa,  ab,  ac,  _pa3,  ab3,  ac3, face(), 1, _lvl+1);
		case 1: return new NodeBranch( ab, _pb,  bc,   ab3, _pb3,  bc3, face(), 1, _lvl+1);
		case 2: return new NodeBranch( bc,  ac,  ab,   bc3,  ac3,  ab3, face(), 1, _lvl+1);
		case 3: return new NodeBranch( ac,  bc, _pc,   ac3,  bc3, _pc3, face(), 1, _lvl+1);
		}
	#else
		switch(t) {
		case 0: return new NodeBranch(_pa,  ab,  ac, face(), 1, _lvl+1);
		case 1: return new NodeBranch( ab, _pb,  bc, face(), 1, _lvl+1);
		case 2: return new NodeBranch( bc,  ac,  ab, face(), 1, _lvl+1);
		case 3: return new NodeBranch( ac,  bc, _pc, face(), 1, _lvl+1);
		}
	#endif
	}

	assert(false);
	return NULL;
}

/////////////////////////////// FIND funcs: //////////////


int NodeBranch::find(NodeVisitor* nv, const vector2f& p, bool bCreate)
{
#ifdef GEO_GLNODES
	nv->visit(this); //visit-branch overload
#endif
	vector2f q = p*2.f-(_pb+_pa);

	if (q*_p1>=0.f) {
		if (_u) return _u->find(nv,p);
		else if (bCreate) {
			_u = create(0);
			 return _u->find(nv,p);
		}
	}

	if (q*_p2>=0.f) {
		if (_l) return _l->find(nv,p);
		else if (bCreate) {
			_l = create(1);
			 return _l->find(nv,p);
		}
	}

	q = p*2.f-(_pb+_pc);  //  /2 pero como solo se usa para ><0 ..

	if (q*_p3>=0.f) {
		if (_c) return _c->find(nv,p);
		else if (bCreate) {
			_c = create(2);
			 return _c->find(nv,p);
		}
	}

	if (_r) return _r->find(nv,p);
	else if (bCreate) {
		_r = create(3);
		 return _r->find(nv,p);
	}

	return 0;
}

int NodeBranch::findAll(NodeVisitor* nv)
{
#ifdef GEO_GLNODES
	nv->visit(this); //visit-branch overload
#endif
	int ret=0;
	if (_u) ret += _u->findAll(nv);
	if (_l) ret += _l->findAll(nv);
	if (_c) ret += _c->findAll(nv);
	if (_r) ret += _r->findAll(nv);
	return ret;
}

int NodeBranch::find(NodeVisitor* nv, const vector2f& p, float r)
{
	int ret=0, ru=0,rl=0,rc=0,rr=0;
	vector2f ab=(_pa+_pb)/2.f;
	vector2f ac=(_pa+_pc)/2.f;
	vector2f bc=(_pb+_pc)/2.f;
	float r2=r*r;

	if (_u)
		ret += find_tri(nv, _pa,ab,ac ,p,r,r2, _u);

	if (_l)
		ret += find_tri(nv, ab,_pb,bc ,p,r,r2, _l);

	if (_c)
		ret += find_tri(nv, bc,ac,ab ,p,r,r2, _c);

	if (_r)
		ret += find_tri(nv, ac,bc,_pc ,p,r,r2, _r);

#ifdef GEO_GLNODES
	if (ret>0)
		nv->visit(this); //visit-branch overload
#endif

	return ret;
}


//ellipse_radius() returns the ellipse radius in the direction specified by v
inline float ellipse_radius2(const vector2f& v, // direction vector relative to center
							 const vector2f& eje_may, //los ejes tienen q ser v unitarios
							 const vector2f& eje_min, 
							 const float a, //r_may, 
							 const float b) //r_min)
{
	// TODO optimizar con otro metodo mejor
	const float theta = atanf((v*eje_min)/(v*eje_may));
	const float a2=a*a, b2=b*b;
	return a2*b2/( b2*cos(theta)*cos(theta) + a2*sin(theta)*sin(theta) );
}


int NodeBranch::find_tri(NodeVisitor* nv, const vector2f& a, const vector2f& b, const vector2f& c,
						const vector2f& p, const float& rxxx, const float& r2, Node* node)
{
	// TODO: opts, share vertex info, dists etc in some way
	assert(node!=NULL);
#ifdef GEO_GLNODES
	nv->visit(this); //visit-branch overload
#endif

	// TODO: usar elipse como resultado de proyectar desde la superficie de la tierra
	//	al triangulo raiz, no circunferenacia simple como ahora. Para hacerlo más facil aqui
	//	se puede calcular el radio vector maximo p-a p-b p-c y usarlo como r (asi se seguiria
	//	usando internamente circunferencia, pero se necesita especificar la elipse proy 2d v,ra,rb)
	

const vector2f eje_may(1.f, 0.f);	// como si fuera circunferencia, para ver q no hay nada roto
const vector2f eje_min(0.f, 1.f);	// update: tiene buena pinta, parece q funciona perfect
const float r_may=rxxx, r_min=rxxx;	//

	const vector2f pa = a-p;
	const float da = pa.norm2();
	const float ra2 = ellipse_radius2(pa, eje_may, eje_min, r_may, r_min); //r2; //calcular segun params de elipse

	const vector2f pb = b-p;
	const float db = pb.norm2();
	const float rb2 = ellipse_radius2(pb, eje_may, eje_min, r_may, r_min); //r2;

	const vector2f pc = c-p;
	const float dc = pc.norm2();
	const float rc2 = ellipse_radius2(pc, eje_may, eje_min, r_may, r_min); //r2;
	
	float r = sqrtf( max(max(ra2,rb2),rc2) ); //para el caso de q no esten los vertices dentro, cogemos 
											// una aproximacion: el r mayor de todos, que cubre seguro

	if (da<ra2 && db<rb2 && dc<rc2)
		return node->findAll(nv);		// todos dentro, el circulo contiene al nodo entero, recorremos entero
	else if (da<ra2 || db<rb2 || dc<rc2)
		return node->find(nv, p, r); 	// alguno dentro, entramos a find
	else if ((p*3.f-(a+b+c)).norm2() < (r+_cr)*(r+_cr)*3.f) // comp dist circulos (P-r y circulo con triang inscrito)
	{													// ^optimizacion para no seguir, no es necesario,
		// ^ todo: falta comprobar mas exhaustivamente este ultimo if q es solo opt
		/// TODO opt sqrts de normalize al menos
		float dm;
		int nd=0;

		vector2f q = p-b;
		vector2f vp = (b-a).rotate_left();

		vp.normalize();
		if (q*vp>=0.f) nd++; else dm=q*vp;

		vp = (c-b).rotate_left();
		vp.normalize();
		if (q*vp>=0.f) nd++; else dm=q*vp;

		if (nd>=1)	// opt para no seguir
		{
			q = p-c;
			vp = (a-c).rotate_left();
			vp.normalize();
			if (q*vp>=0.f) 
				nd++; 
			else if (nd==2)
				dm = q*vp;

			if (nd==3 || (nd==2 && dm>-r)) // nd3 => P dentro del triangulo, entramos a find
				return node->find(nv, p, r);	// nd2&dm>-r => el circulo roza por un lado 
											// nd1 => tocaria vertice si acaso y ya comprobamos antes q no tocaba vertices
											// nd0 => imposible
		}
	}
	
	return 0;
}



NodeBranch::~NodeBranch() 
{
	if (_r) delete _r;
	if (_c) delete _c;
	if (_l) delete _l;
	if (_u) delete _u;
}

