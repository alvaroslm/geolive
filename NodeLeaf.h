
// (C) Alvaro Salmador 2011
//  All rights reserved

#ifndef __NODELEAF_H_INCLUIDO_
#define __NODELEAF_H_INCLUIDO_

#include "NodeBranch.h"

class NodeLeaf : public Node, public obj_counter<NodeLeaf>
{
	friend class NodeBranch;
public:
	NodeLeaf(NodeBranch* np, const vector2f& a, const vector2f& b, const vector2f& c)
		: Node(true,np->face()), _np(np), _pa(a), _pb(b), _pc(c) {}

	//int		find(vector<User*>& l, const vector3f& p, float r); early ideas
	//int		find(list<Msg*>& l, const vector3f& p, float r);
	//int		find(NodeVisitor* nv, const vector3f& p, float r);
	//int		findAll(NodeVisitor* nv);

	inline int	find(NodeVisitor* nv, const vector2f& p, float r) {
		nv->visit(this, p, r);
		return 1;
	}

	inline int	find(NodeVisitor* nv, const vector2f& p) {
		nv->visit(this, p);
		return 1;
	}

	inline int	findAll(NodeVisitor* nv) {	
		nv->visit(this);
		return 1;
	}


#ifdef GEO_GLNODES
	virtual void draw(int lvl, const vector3f& eye) const;
#endif

private:
	NodeBranch*		_np;
	// this one actually has data (sample msg and user
	// TODO think of containers to use too
	deque<User*>	_ul;
	deque<Msg*>		_ml;
	vector2f		_pa, _pb, _pc;
	
	NodeLeaf() : Node(true,0) {}
};

#endif