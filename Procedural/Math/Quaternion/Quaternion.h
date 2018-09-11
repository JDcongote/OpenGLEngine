#pragma once
#include"../Math3D.h"
#include "../Matrix/Matrix4f.h"
class Quaternion
{
public:
	Quaternion();
	Quaternion(const float angle, const float x, const float y, const float z);
	Matrix4f get_matrix();

	Quaternion operator * (Quaternion&);

	Vector4f versor();
	void set_versor(Vector4f versor);
	void normalize();
	~Quaternion();

private:
	// Versor unit vector
	Vector4f q;
	Matrix4f rot_matrix;
};

