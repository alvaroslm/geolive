
// (C) Alvaro Salmador 2011
//  All rights reserved

#ifndef __NODEROOT_H_INCLUIDO_
#define __NODEROOT_H_INCLUIDO_

#include "Node.h"

class NodeRoot : public Node
{
public:
	NodeRoot(int nlev);
	~NodeRoot();

	static const int NROOT=20;

	int find(NodeVisitor* nv, float lat, float lon, float r=-1.f);
	int findAll(NodeVisitor* nv);

	inline vector2f vto2(int i, const vector3f& p) const {
		const float k = (_v1[i]*_normal[i])/(p*_normal[i]);
		const vector3f q = p*k - _v1[i];
		// pto interseccion con el plano cara del triangulo, luego proyectado sobre los ejes xy
		// importante hacerlo asi y no proyectando la interseccion con la esfera
		return vector2f(q*_ax[i], q*_ay[i]);
	}

	inline vector3f vto3(int i, const vector2f& p) const {
		// punto 3D en la cara del triangulo correspondiente
		return(p.x*_ax[i] + p.y*_ay[i] + _v1[i]);
	}

#ifdef GEO_GLNODES
	virtual void draw(int depth, const vector3f& eye) const;
#endif
	//virtual void draw(int depth, const vector3f& eye) const {}
	

private:
	NodeRoot() : Node(false,0) {}

	inline int find_branch(int nbranch, NodeVisitor* nv, const vector3f& P, float r, const float* dedge);

#ifdef GEO_GLNODES
	void	draw_circ(const vector3f& p, float r, int nface) const;
#endif

	NodeBranch*	_branch[20];

	Bitmask		_b[8];			// partition planes "discard maps"
	vector3f	_pedge[20][3];	// plane vectors for each 3d *edge* of each face
	vector3f	_ax[20], _ay[20];	// ejes 2D para cambiar a 2D
	vector3f	_v1[20], _v2[20], _v3[20]; // vertices de cada cara
	vector3f	_normal[20];	// normal de cada cara
	int			_neighbour[20][3]; // triangulo vecino al [triangulo][arista] seleccionado

#ifdef GEO_GLNODES
	vector3f	_findP, _findP2; //last find
	float		_findR;
	int			_findi;
#endif
};

#endif