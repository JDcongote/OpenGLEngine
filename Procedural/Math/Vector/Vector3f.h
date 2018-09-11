#pragma once
#include "Vector.h"
class Vector3f : public Vector
{
public:
	Vector3f();
	Vector3f(const float x ,const float y, const float z);

	const float dot(Vector3f v) const;
	const Vector3f normalize();
	const Vector3f cross(Vector3f v) const;
	const float* get_as_array() const;
	Vector3f operator - (const Vector3f &v);
	Vector3f operator + (const Vector3f &v);	
	Vector3f operator * (Vector3f &v);
	Vector3f operator * (const float &s);
	float operator [] (const int index) const;
	
	const float magnitude();


	~Vector3f();
private:
	float vec[3];
};

