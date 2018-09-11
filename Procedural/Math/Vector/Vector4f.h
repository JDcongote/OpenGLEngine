#pragma once
#include "Vector.h"
class Vector4f :
	public Vector
{
public:
	Vector4f();
	Vector4f(const float x, const float y, const float z, const float w);

	const float dot(Vector4f v) const;
	Vector4f operator * (Vector4f &v);
	Vector4f operator + (Vector4f &v);
	
	const Vector4f normalize();
	const Vector4f cross(Vector4f v) const;
	const float magnitude();


	~Vector4f();
private:
	float vec[4];
};

