#pragma once
#include "../Math3D.h"
class Vector
{
public:
	Vector(float...);
	Vector();

	const Vector normalize()const;
	const float magnitude();
	const float dot(Vector v) const;
	const Vector cross(Vector v) const;

	// operators
	Vector operator [] (Vector&);
	Vector operator * (Vector&);
	Vector operator * (float&);

	float x, y, z, w, xy, xyz, xyzw;
	float magn;

	~Vector();
};