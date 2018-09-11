#pragma once
#include "../Math3D.h"
#include "../Vector/Vector4f.h"
class Matrix4f
{
public:
	Matrix4f();
	Matrix4f(int zeros);
	Matrix4f(const Vector4f col0, const Vector4f col1, const Vector4f col2, const Vector4f col3);
	~Matrix4f();
	

	//Members
	float get(int index) const;	
	float* get_as_array();
	Matrix4f inverse() const;
	float determinant() const;

	void translate(const float x, const float y, const float z);
	void translate_x(const float x);
	void translate_y(const float y);
	void translate_z(const float z);
	void scale(const float x, const  float y, const float z);
	void rotate(const float x, const float y, const float z);
	void rotate_x(const float x);
	void rotate_y(const float y);
	void rotate_z(const float z);

	void set_position(const float x, const float y, const float z);
	void set_scale(const float x, const float y, const float z);
	void set_rotation(const float x, const float y, const float z);

	//Parameters
	float m11;
	float m12;
	float m13;
	float m14;
	float m21;
	float m22;
	float m23;
	float m24;
	float m31;
	float m32;
	float m33;
	float m34;
	float m41;
	float m42;
	float m43;
	float m44;

	//operators
	float& operator [] (const int index);
	Matrix4f operator * (Matrix4f&);
	Vector4f operator * (Vector4f&);

private:
	float mat[16];	
	int size_t = 16;
	void _update();
};

