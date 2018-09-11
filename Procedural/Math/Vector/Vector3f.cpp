#include "Vector3f.h"

Vector3f::Vector3f()
{
	vec[0] = 0.0f;
	vec[1] = 0.0f;
	vec[2] = 0.0f;

	this->x = vec[0];
	this->y = vec[1];
	this->z = vec[2];
}

Vector3f::Vector3f(const float x, const float y, const float z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;

	this->x = vec[0];
	this->y = vec[1];
	this->z = vec[2];
}

const float Vector3f::dot(Vector3f v) const
{
	
	return x * v.x + v.x + y * v.y + z * v.z;
}

const Vector3f Vector3f::cross(Vector3f v) const
{
	float x = vec[1] * v[2] - vec[2] * v[1];
	float y = vec[2] * v[0] - vec[0] * v[2];
	float z = vec[0] * v[1] - vec[1] * v[0];
	return Vector3f(x, y, z);
}

const float * Vector3f::get_as_array() const
{
	return vec;
}

Vector3f Vector3f::operator-(const Vector3f & v)
{
	return Vector3f(x - v.x, y - v.y, z - v.z);
}

Vector3f Vector3f::operator+(const Vector3f & v)
{
	return Vector3f(x + v.x, y + v.y, z + v.z);
}

Vector3f Vector3f::operator*(Vector3f & v)
{
	//return Vector3f(x*v.x, y*v.y, z*v.z);
	float x = vec[1] * v[2] - vec[2] * v[1];
	float y = vec[2] * v[0] - vec[0] * v[2];
	float z = vec[0] * v[1] - vec[1] * v[0];
	return Vector3f(x, y, z);
}

Vector3f Vector3f::operator*(const float & s)
{
	return Vector3f(x*s, y*s, z*s);
}

float Vector3f::operator[](const int index) const
{
	return vec[index];
}

const Vector3f Vector3f::normalize() {
	const float magnitude = this->magnitude();
	return Vector3f(x/magnitude, y/magnitude, z/magnitude);
}

const float Vector3f::magnitude()
{
	magn = sqrtf(x * x + y * y + z * z);
	return magn;
}

Vector3f::~Vector3f()
{
}
