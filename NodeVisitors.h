
// (C) Alvaro Salmador 2011
//  All rights reserved


#ifndef _NODEVISITORS_H_INCLUIDO_
#define _NODEVISITORS_H_INCLUIDO_ 1

#include "Node.h"
#include "NodeBranch.h"
#include "NodeLeaf.h"

#include "Msg.h"
#include "User.h"

class NullNodeVisitor : public NodeVisitor 
{
public:
	virtual void visit(NodeLeaf* n, const vector2f& p, float r) {}
};

class MarkNodeVisitor : public NodeVisitor 
{
public:
	MarkNodeVisitor(bool bMark=true) : _bMark(bMark) {}

	virtual void visit(NodeLeaf* n) {
	#ifdef GEO_GLNODES
		n->_bColorMark = _bMark;
	#endif
	}

	virtual void visit(NodeLeaf* n, const vector2f& p, float r) {
	#ifdef GEO_GLNODES
		n->_bColorMark = _bMark;
	#endif
	}

	virtual void visit(NodeLeaf* n, const vector2f& p) {
	#ifdef GEO_GLNODES
		n->_bColorMark = _bMark;
	#endif
	}

	virtual void visit(NodeBranch* n) {
	#ifdef GEO_GLNODES
		n->_bColorMark = _bMark;
	#endif
	}

protected:
	bool	_bMark;
};


class NuserNodeVisitor : public NodeVisitor 
{
public:
	NuserNodeVisitor(User* usr) : _usr(usr) {}

	virtual void visit(NodeLeaf* n, const vector2f& p, float r) {
	#ifdef GEO_GLNODES
		n->_bColorMark = true;
	#endif
		// search msgs & send all to user. 
	}

private:
	User*	_usr;
};



class NmsgNodeVisitor : public NodeVisitor 
{
public:
	NmsgNodeVisitor(Msg* msg) : _msg(msg) {}

	virtual void visit(NodeLeaf* n, const vector2f& p, float r) {
#ifdef GEO_GLNODES
		n->_bColorMark = true;
#endif
		// search users & send them the msg & insert msg in appropriate leaf (& gc?)
	}

private:
	Msg*	_msg;
};


/* LOCKING para cuando hay q hacerlo:
	lectura=uso "normal"; escritura es reestructurar el arbol (quitar/poner nodos)
// en la entrada de cualquier funcion de lectura de un nodebranch:
while(true) 
{
	while(writelock) yield;
	readers++;  <atomic
	if (!writelock)    
		break;
	else
		readers--;  <atomic
}
-read whatever-
readers--;

/////////////////

// en cualquier func de escritura:
lock(writelock); //waits til available
while(readers>0) yield;
-read/write whatever-
unlock(writelock);

*/


#endif