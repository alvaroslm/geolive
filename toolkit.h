
// (C) Alvaro Salmador 2011
//  All rights reserved

#ifndef _TOOLKIT_H_INCLUIDO_
#define _TOOLKIT_H_INCLUIDO_ 1

template<typename T> inline T deg2rad(T d) {
	return d*PI/180.;
}
template<> inline float deg2rad<float>(float d) {
	return d*F_PI/180.f; 
}
template<typename T> inline T rad2deg(T d) {
	return d*180./PI;
}
template<> inline float rad2deg<float>(float d) {
	return d*180.f/F_PI; 
}


class Bitmask {
public:
	explicit Bitmask() : _mask(0xFFFFFFFF) {}
	Bitmask(unsigned int mask) : _mask(mask) {}

	inline Bitmask& operator=(unsigned int mask) {
		_mask = mask;
		return *this;
	}

	inline Bitmask& operator&=(unsigned int mask) {
		_mask &= mask;
		return *this;
	}

	inline Bitmask& operator|=(unsigned int mask) {
		_mask |= mask;
		return *this;
	}

	inline  operator const unsigned int() const {
		return _mask;
	}

	inline const bool operator[](int i) const {
		return (_mask&(1<<i)) != 0;
	}

	inline void set_and(int i, bool b) {
		if (!b)
			_mask &= ~(1<<i);
	}

	inline void set_or(int i, bool b) {
		if (b)
			_mask |= (1<<i);
	}

	inline void set(int i) {
		_mask |= (1<<i);
	}

	inline void unset(int i) {
		_mask &= ~(1<<i);
	}

	inline void set(int i, bool b) {
		if (b)
			_mask |= (1<<i);
		else
			_mask &= ~(1<<i);
	}

protected:
	unsigned int _mask;
};


template <typename T> struct obj_counter 
{
	inline obj_counter() {
        //++Created;
        ++N;
    }
 
    virtual ~obj_counter() {
        --N;
    }
    
	//static int Created;
    static unsigned int N;
};
//template <typename T> int obj_counter<T>::Created(0);
template <typename T> unsigned int obj_counter<T>::N(0);


template <typename T> struct obj_counter2 : public obj_counter<T>
{
	inline obj_counter2() : obj_counter() {
        ++Created;
    }
   
	static unsigned long long Created;
};
template <typename T> unsigned long long obj_counter2<T>::Created(0L);

#endif