
// (C) Alvaro Salmador 2011
//  All rights reserved


#ifndef __MSG_H_INCLUIDO_
#define __MSG_H_INCLUIDO_ 1

#include <string.h>
#include <list>
#include <string>
#include <deque>
#include <vector>
using namespace std;

//hay q considerar hacerlos smart pointers o similar para el borrado 


class Msg
{
	Msg(const char* msg) {
		size_t l=strlen(msg)+1;
		_m = new char [l];
		strncpy(_m, msg, l); 
	}

	~Msg() {
		delete _m;
	}

	//data
	char*	_m;
	float _lat, _lon;

private:
	Msg() {}
};


#endif