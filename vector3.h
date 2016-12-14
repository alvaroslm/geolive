
// (C) Alvaro Salmador 2002-2011
//  All rights reserved


#ifndef _VECTOR3_H_INCLUIDO_
#define _VECTOR3_H_INCLUIDO_

#include <math.h>

template <typename T> 
struct vector3 
{
	inline vector3() {}   // constructores
	explicit inline vector3(const T X, const T Y, const T Z) : x(X), y(Y), z(Z) {}
	explicit inline vector3(const T *vf) { 
		x=vf[0]; y=vf[1]; z=vf[2]; 
	}
	template <typename X> inline vector3(const vector3<X>& v) {
		x=(T)v.x; y=(T)v.y; z=(T)v.z;
	}

	// acceso al array
	inline	T&	operator [] (const int n) { 
		switch(n) { 
		case 0:	return x;
		case 1:	return y;
		case 2:	return z;
		default:
			_ASSERT(false);
			return x;
		}
	}

	inline const vector3<T>& operator = (const vector3<T>& v) {
		x=v.x; y=v.y; z=v.z;
		return *this;
	}

	inline const vector3<T>& operator=(const T* v3f) {
		x=v3f[0];  y=v3f[1]; z=v3f[2];
		return *this;
	}

	inline bool operator==(const vector3<T>& v) const {
		return x==v.x && y==v.y && z==v.z;
	}

	inline bool operator!=(const vector3<T>& v) const
	{
		return x!=v.x || y!=v.y || z!=v.z;
	}

	// producto escalar
	inline	T operator * (const vector3<T>& v1) const { 
		return(x*v1.x + y*v1.y + z*v1.z); 
	}

	// producto por un escalar (v3*esc)
	inline	vector3<T> operator * (const T k) const { 
		return vector3<T>(x*k, y*k, z*k); 
	}

	// *= un escalar
	inline	const vector3<T>& operator *= (const T k) { 
		x*=k; y*=k; z*=k; 
		return *this; 
	}

	// division por un escalar
	inline	vector3<T> operator / (const T k) const { 
		return vector3<T>(x/k, y/k, z/k); 
	}

	// /= un escalar
	inline	const vector3<T>& operator /= (const T k) { 
		x/=k; y/=k; z/=k; 
		return *this; 
	}

	// suma
	inline	vector3<T> operator + (const vector3<T> & v1) const { 
		return vector3<T>(x + v1.x, y + v1.y, z + v1.z); 
	}

	// suma y asignacion
	inline	const vector3<T>& operator += (const vector3<T>& v1) {
		x+=v1.x; y+=v1.y; z+=v1.z;
		return *this;
	}

	// resta
	inline	vector3<T> operator - (const vector3<T>& v1) const {
		return vector3<T>(x - v1.x, y - v1.y, z-v1.z);
	}

	// resta y asignacion
	inline	const vector3<T>& operator -= (const vector3<T>& v1) {
		x-=v1.x; y-=v1.y; z-=v1.z;
		return *this;
	}

	// producto vectorial
	inline	vector3<T> operator ^ (const vector3<T>& u) const {
		return vector3<T>(	y*u.z - z*u.y,
							z*u.x - x*u.z,
							x*u.y - y*u.x );
	}

	// prod vectorial y asignacion
	inline	const vector3<T>& operator ^= (const vector3<T>& u) {
		register T vx, vy;
		vx = y*u.z - z*u.y;
		vy = z*u.x - x*u.z; 
		z = x*u.y - y*u.x;
		y = vy;
		x = vx;
		return *this;
	}

	inline  T	norm() const; // modulo (norma)
	inline  T	norm2() const { // modulo^2
		return(x*x + y*y + z*z); 
	}	

	// coseno del ángulo con otro vector
	inline	T	cos(const vector3<T>& u) const;

	// ángulo con otro vector
	inline	T	ang(const vector3<T>& u) const;
	
	// exportar a un array de T
	inline	void exportar(T *v3d) const { 
		v3d[0]=x; v3d[1]=y; v3d[2]=z; 
	}
	
	// unary -
	inline	vector3<T>	operator-()	const {
		return vector3<T>(-x, -y, -z); 
	}
	
	// valores absolutos de las componentes
	inline	const vector3<T>&	abs();
	
	// normalizar el vector
	inline	void	normalize();	
	inline	void	normalize(T k);

	T	x, y, z;	// valor
};


//// producto por un escalar (esc*v3)
template<typename T> inline	vector3<T> operator * (const T k, const vector3<T>& v) { 
	return vector3<T>(v.x*k, v.y*k, v.z*k); 
}


///////// float specializations:


template<typename T> inline T vector3<T>::norm() const {
	return sqrt(x*x + y*y + z*z); 
}

template<> inline float vector3<float>::norm() const {
	return sqrtf(x*x + y*y + z*z); 
}



template<typename T> inline	const vector3<T>&	vector3<T>::abs() { 
	x=abs(x); y=abs(y); z=abs(z); 
	return *this; 
}

template<> inline const vector3<float>&	vector3<float>::abs() {
	x=fabsf(x); y=fabsf(y); z=fabsf(z); 
	return *this; 
}



template<typename T> inline T vector3<T>::ang(const vector3<T>& u) const {
	return acos( (x*u.x + y*u.y + z*u.z) / (sqrt(u.x*u.x + u.y*u.y + u.z*u.z)*norm()) );
}

template<> inline float vector3<float>::ang(const vector3<float>& u) const {
	return acosf( (x*u.x + y*u.y + z*u.z) / (sqrtf(u.x*u.x + u.y*u.y + u.z*u.z)*norm()) );
}



template<typename T> inline T vector3<T>::cos(const vector3<T>& u) const {
	return( (x*u.x + y*u.y + z*u.z) / (sqrt(u.x*u.x + u.y*u.y + u.z*u.z)*norm()) );
}

template<> inline float vector3<float>::cos(const vector3<float>& u) const {
	return( (x*u.x + y*u.y + z*u.z) / (sqrtf(u.x*u.x + u.y*u.y + u.z*u.z)*norm()) );
}



template<typename T> inline void vector3<T>::normalize() {	
	register T md = sqrt(x*x + y*y + z*z);
	x/=md;	y/=md;	z/=md;
}

template<> inline void vector3<float>::normalize() {	
	register float md = sqrtf(x*x + y*y + z*z);
	x/=md;	y/=md;	z/=md;
}

template<typename T> inline void vector3<T>::normalize(T k) {	
	register T md = k/sqrt(x*x + y*y + z*z);
	x*=md; y*=md; z*=md;
}

template<> inline void vector3<float>::normalize(float k) {	
	register float md = k/sqrtf(x*x + y*y + z*z);
	x*=md; y*=md; z*=md;
}


////////////


template <typename T> inline T	v3CalcDet(const T* a, const T* b, const T* c) 
{
	return (	( a[0]*b[1]*c[2] + b[0]*c[1]*a[2] + a[1]*b[2]*c[0] ) -
				( c[0]*b[1]*a[2] + b[0]*a[1]*c[2] + c[1]*b[2]*a[0] )	);
}

typedef vector3<float>	vector3f;
typedef vector3<double>	vector3d;


#endif	// _VECTOR3_H_INCLUIDO_
