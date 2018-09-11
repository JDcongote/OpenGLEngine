#include "Vector4f.h"


Vector4f::Vector4f()
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;
	vec[3] = 0.0f;

	this->x = vec[0];
	this->y = vec[1];
	this->z = vec[2];
	this->w = vec[3];
}

Vector4f::Vector4f(const float x, const float y, const float z, const float w)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
	vec[3] = w;

	this->x = vec[0];
	this->y = vec[1];
	this->z = vec[2];
	this->w = vec[3];
}

const float Vector4f::dot(Vector4f v) const
{
	return x * v.x + v.x + y * v.y + z * v.z + w * v.w;
}

Vector4f Vector4f::operator * (Vector4f &v)
{
	return Vector4f(x*v.x, y*v.y, z*v.z, w * v.w);
}

Vector4f Vector4f::operator+(Vector4f & v)
{
	return Vector4f(x+v.x,y+v.y,z+v.z,w+v.w);
}

const Vector4f Vector4f::cross(Vector4f v) const
{
	return Vector4f((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x), (x * v.y) - (y * v.x));
}

const Vector4f Vector4f::normalize() {
	const float magnitude = this->magnitude();
	return Vector4f(x / magnitude, y / magnitude, z / magnitude, w / magnitude);
}

const float Vector4f::magnitude()
{
	magn = sqrtf(x * x + y * y + z * z + w * w);
	return magn;
}

Vector4f::~Vector4f()
{
}
