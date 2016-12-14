
// (C) Alvaro Salmador 2011
//  All rights reserved

#ifndef __NODEBRANCH_H_INCLUIDO_
#define __NODEBRANCH_H_INCLUIDO_ 1

#include <deque>
using namespace std;

class NodeRoot;

class NodeBranch : public Node
{
	friend class NodeLeaf;
public:
	NodeBranch(	const vector2f& a, const vector2f& b, const vector2f& c, 
	#ifdef GEO_GLNODES
			    const vector3f& a3, const vector3f& b3, const vector3f& c3,
	#endif
				char face, int nlev, int lvl);
	~NodeBranch();

	Node* create(char t);

	int findAll(NodeVisitor* nv);
	int find(NodeVisitor* nv, const vector2f& p, bool bCreate=true);
	int find(NodeVisitor* nv, const vector2f& p, float r);

	inline char level() const {
		return _lvl;
	}

#ifdef GEO_GLNODES
	friend class NodeRoot;
	void draw(int depth, const vector3f& eye) const;
#endif

private:
	int find_tri(NodeVisitor* nv, const vector2f& a, const vector2f& b, const vector2f& c,
						const vector2f& p, const float& rxxx, const float& r2, Node* node);
	
	Node		*_u, *_l, *_c, *_r;
	vector2f	_p1, _p2, _p3; // planos q separan hijos
	vector2f	_pa, _pb, _pc; // los 3 puntos del sector triangular
#ifdef GEO_GLNODES
	vector3f	_normal;
	vector3f	_pa3, _pb3, _pc3; // los 3 puntos del sector triangular
	float		_cr;
#endif
	char		_lvl;
};


/*	if (_lvl>1) 
		return 0;

	extern deque<pair<vector3f,vector3f> > listapuntos;

	//listapuntos.clear();
	vector3f rojo(1.f,0.f,0.f);
	vector3f verde(0.f,1.f,0.f);
	vector3f azul(0.f,0.f,1.f);
	vector3f blanco(1.f,1.f,1.f);

	listapuntos.push_back(make_pair(vto3(a),rojo));
	listapuntos.push_back(make_pair(vto3(b),verde));
	listapuntos.push_back(make_pair(vto3(c),azul));
	listapuntos.push_back(make_pair(vto3(q+b),blanco));
*/

#endif