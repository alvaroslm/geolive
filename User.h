
// (C) Alvaro Salmador 2011
//  All rights reserved

#ifndef __USER_H_INCLUIDO_
#define __USER_H_INCLUIDO_ 1

#include <string.h>
#include <list>
#include <string>
#include <deque>
#include <vector>
using namespace std;


class Msg;

class User
{
public:
	User(unsigned int i) : _uid(i) {}

	//data
	float _lat, _lon;
	//xyz calculados?

private:
	User() {}
	unsigned int _uid;
};



#endif