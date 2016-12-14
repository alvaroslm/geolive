
// (C) Alvaro Salmador 2011
//  All rights reserved

#ifndef __NODE_H_INCLUIDO_
#define __NODE_H_INCLUIDO_ 1

#include "global_defs.h"
#include "vector3.h"
#include "vector2.h"

#include <cassert>
#include <list>
#include <vector>
#include <deque>
using namespace std;

class User;
class Msg;
class NodeBranch;
class NodeRoot;
class NodeLeaf;
class NodeVisitor;

inline vector3f latlon2vector(float lat, float lon) {
	lat = deg2rad(lat);
	lon = deg2rad(lon);
	const float Rp = F_GEO_R*cosf(lat);
	return vector3f(Rp*cosf(lon), Rp*sinf(lon), F_GEO_R*sinf(lat));
}

inline float rreal2rsphere(const float& rr) {
	// convierte de distancia real curvada recorriendo la superficie de la esfera desde P
	// al equivalente en forma de radio de otra esfera centrada en P que cubre el casquete
	// en realidad esto es prescindible porque la diferencia es despreciable a distancias cortas
	// pero bueno, a distancias de 6000km puede ser 200km de diferencia
	return F_GEO_R2*sinf(rr/F_GEO_R2);
}

class Node : public obj_counter<Node>
{
public:

	template<class T> inline int find(T* nv, const vector3f& p, float r) 
	{	//templated to force parsing of NodeBranch/Leaf
		if (!isLeaf())
			return static_cast<NodeBranch*>(this)->find(nv,p,r);
		else
			return static_cast<NodeLeaf*>(this)->find(nv,p,r);
	}

	template<class T> inline int find(T* nv, const vector3f& p) 
	{
		if (!isLeaf())
			return static_cast<NodeBranch*>(this)->find(nv,p);
		else
			return static_cast<NodeLeaf*>(this)->find(nv,p);
	}

	template<class T> inline int find(T* nv, const vector2f& p, float r) 
	{	//templated to force parsing of NodeBranch/Leaf
		if (!isLeaf())
			return static_cast<NodeBranch*>(this)->find(nv,p,r);
		else
			return static_cast<NodeLeaf*>(this)->find(nv,p,r);
		return 0;
	}

	template<class T> inline int find(T* nv, const vector2f& p) 
	{
		if (!isLeaf())
			return static_cast<NodeBranch*>(this)->find(nv,p);
		else
			return static_cast<NodeLeaf*>(this)->find(nv,p);
		return 0;
	}

	template<class T> inline int findAll(T* nv) 
	{
		if (!isLeaf())
			return static_cast<NodeBranch*>(this)->findAll(nv);
		else
			return static_cast<NodeLeaf*>(this)->findAll(nv);
	}

	inline char face() const {
		return _bits&0x1f;
	}

	inline bool isLeaf() const {
		return (_bits&0x20)!=0;
	}

	template<class T> inline vector2f vto2(const T& p) const {
		return Root->vto2(face(), p);
	}

	template<class T> inline vector3f vto3(const T& p) const {
		return Root->vto3(face(), p);
	}

#ifdef GEO_GLNODES
	virtual void draw(int depth, const vector3f& eye) const=0;
	bool	_bColorMark;
#endif

protected:
	explicit Node(bool isleaf, int nface) : _bits(nface|(isleaf?0x20:0))
#ifdef GEO_GLNODES
	,_bColorMark(false)
#endif 
	{}

#ifdef GEO_GLNODES
	void	draw_arc(vector3f a, vector3f b, float* color=0, float lwidth=1.f, int over=0) const;
#endif

	const char	_bits; //root face, isleaf
	static NodeRoot* Root;

private:
	Node() : _bits(0) {}
};

////////


class NodeVisitor { //interface
public:
	virtual void visit(NodeLeaf* n, const vector2f& p, float r)=0;
	virtual void visit(NodeLeaf* n, const vector2f& p) {}
	virtual void visit(NodeLeaf* n) {}
	virtual void visit(NodeBranch* n) {}
	//virtual ~NodeVisitor() {}
};


////////////////////////////////////////


#endif
