
/* (c) Alvaro Salmador 2002-2011 */


#ifndef _VECTOR2_H_INCLUIDO_
#define _VECTOR2_H_INCLUIDO_

#include <math.h>

template <typename T> 
struct vector2 
{
	inline vector2() {}   // constructores
	explicit inline vector2(const T X, const T Y) : x(X), y(Y) {}
	explicit inline vector2(const T *vf) { 
		x=vf[0]; y=vf[1];  
	}

	// acceso al array
	inline	T&	operator [] (const int n) { 
		if (n==0)
			return x;
		else
			return y;
	}

	inline const vector2<T>& operator = (const vector2<T>& v) {
		x=v.x; y=v.y;
		return *this;
	}

	inline const vector2<T>& operator=(const T* v3f) {
		x=v3f[0];  y=v3f[1];
		return *this;
	}

	inline bool operator==(const vector2<T>& v) const {
		return x==v.x && y==v.y;
	}

	inline bool operator!=(const vector2<T>& v) const
	{
		return x!=v.x || y!=v.y;
	}

	inline const vector2<T>& rotate_left() {
		register const T t = x;
		x = -y;
		y = t;
		return *this;
	}

	inline const vector2<T>& rotate_right() {
		register const T t = y;
		y = -x;
		x = t;
		return *this;
	}

	// producto escalar
	inline	T operator * (const vector2<T>& v1) const { 
		return(x*v1.x + y*v1.y); 
	}

	// producto por un escalar
	inline	vector2<T> operator * (const T k) const { 
		return vector2<T>(x*k, y*k); 
	}

	// *= un escalar
	inline	const vector2<T>& operator *= (const T k) { 
		x*=k; y*=k;
		return *this; 
	}

	// division por un escalar
	inline	vector2<T> operator / (const T k) const { 
		return vector2<T>(x/k, y/k); 
	}

	// /= un escalar
	inline	const vector2<T>& operator /= (const T k) { 
		x/=k; y/=k;
		return *this; 
	}

	// suma
	inline	vector2<T> operator + (const vector2<T> & v1) const { 
		return vector2<T>(x + v1.x,  y + v1.y); 
	}

	// suma y asignacion
	inline	const vector2<T>& operator += (const vector2<T>& v1) {
		x+=v1.x; y+=v1.y;
		return *this;
	}

	// resta
	inline	vector2<T> operator - (const vector2<T>& v1) const {
		return vector2<T>(x - v1.x, y - v1.y);
	}

	// resta y asignacion
	inline	const vector2<T>& operator -= (const vector2<T>& v1) {
		x-=v1.x; y-=v1.y;
		return *this;
	}

	inline  T	norm() const; // modulo (norma)
	inline  T	norm2() const { // modulo^2
		return x*x + y*y; 
	}	

	// coseno del ángulo con otro vector
	inline	T	cos(const vector2<T>& u) const;

	// ángulo con otro vector
	inline	T	ang(const vector2<T>& u) const;
	
	// exportar a un array de T
	inline	void exportar(T *v3d) const { 
		v3d[0]=x; v3d[1]=y;
	}
	
	// unary -
	inline	vector2<T>	operator-()	const {
		return vector2<T>(-x, -y); 
	}
	
	// valores absolutos de las componentes
	inline	const vector2<T>&	abs();
	
	// normalizar el vector
	inline	void	normalize();	
	inline	void	normalize(T k);

	T	x, y;	// valor
};


//// producto por un escalar (esc*v3)
template<typename T> inline	vector2<T> operator * (const T k, const vector2<T>& v) { 
	return vector2<T>(v.x*k, v.y*k); 
}


///////// float specializations:



template<typename T> inline T vector2<T>::norm() const {
	return sqrt(x*x + y*y); 
}

template<> inline float vector2<float>::norm() const {
	return sqrtf(x*x + y*y); 
}



template<typename T> inline	const vector2<T>&	vector2<T>::abs() { 
	x=abs(x); y=abs(y);
	return *this; 
}

template<> inline const vector2<float>&	vector2<float>::abs() {
	x=fabsf(x); y=fabsf(y);
	return *this; 
}



template<typename T> inline T vector2<T>::ang(const vector2<T>& u) const {
	return acos( (x*u.x + y*u.y) / (sqrt(u.x*u.x + u.y*u.y)*norm()) );
}

template<> inline float vector2<float>::ang(const vector2<float>& u) const {
	return acosf( (x*u.x + y*u.y) / (sqrtf(u.x*u.x + u.y*u.y)*norm()) );
}



template<typename T> inline T vector2<T>::cos(const vector2<T>& u) const {
	return( (x*u.x + y*u.y) / (sqrt(u.x*u.x + u.y*u.y)*norm()) );
}

template<> inline float vector2<float>::cos(const vector2<float>& u) const {
	return( (x*u.x + y*u.y) / (sqrtf(u.x*u.x + u.y*u.y)*norm()) );
}



template<typename T> inline void vector2<T>::normalize() {	
	register T md = sqrt(x*x + y*y);
	x/=md;	y/=md;
}

template<> inline void vector2<float>::normalize() {	
	register float md = sqrtf(x*x + y*y);
	x/=md;	y/=md;
}

template<typename T> inline void vector2<T>::normalize(T k) {	
	register T md = k/sqrt(x*x + y*y);
	x*=md; y*=md;
}

template<> inline void vector2<float>::normalize(float k) {	
	register float md = k/sqrtf(x*x + y*y);
	x*=md; y*=md;
}


////////////


typedef vector2<float>	vector2f;
typedef vector2<double>	vector2d;


#endif	// _vector2_H_INCLUIDO_
