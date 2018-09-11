#include "Vector.h"

Vector::Vector()
{
}

const Vector Vector::normalize() const
{
	return Vector();
}

const float Vector::magnitude()
{
	return 0.0f;
}

const float Vector::dot(Vector v) const
{
	return 0.0f;
}

const Vector Vector::cross(Vector v) const
{
	return Vector();
}

Vector Vector::operator[](Vector &)
{
	return Vector();
}

Vector Vector::operator*(Vector &)
{
	return Vector();
}

Vector Vector::operator*(float &)
{
	return Vector();
}



Vector::Vector(float...)
{
}


Vector::~Vector()
{
}
