#include "Quaternion.h"



Quaternion::Quaternion()
{
}

Quaternion::Quaternion(const float angle, const float x, const float y, const float z) 
{
	float angle_rad = angle * ONE_DEG_IN_RAD;
	q = Vector4f(cosf(angle_rad / 2), (sinf(angle_rad / 2)) * x, (sinf(angle_rad / 2)) * y, (sinf(angle_rad / 2)) * z);
}

Matrix4f Quaternion::get_matrix()
{
	float w = q.x;
	float x = q.y;
	float y = q.z;
	float z = q.w;
	
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;

	if ((w*w) + xx + yy + zz != 1) {
		q = q.normalize();
	}

;	return Matrix4f(Vector4f(1.0f - 2.0f * yy - 2.0f * zz, 2.0f * x * y - 2.0f * w * z, 2.0f * x * z + 2.0f * w * y , 0.0f),
					Vector4f(2.0f * x * y + 2.0f * w * z , 1 - 2.0f * xx - 2.0f * zz  , 2.0f * y * z - 2.0f * w * x , 0.0f),
					Vector4f(2.0f * x * z - 2.0f * w * y , 2.0f * y * z + 2.0f * w * x, 1.0f - 2.0f * xx - 2.0f * yy, 0.0f),
					Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
}

Quaternion Quaternion::operator*(Quaternion &q2)
{
	Quaternion q_result = Quaternion();
	q_result.set_versor(Vector4f(	q.x * q2.q.x - q.y * q2.q.y - q.z * q2.q.z - q.w * q2.q.w, 
									q.x * q2.q.y + q.y * q2.q.x - q.z * q2.q.w + q.w * q2.q.z,
									q.x * q2.q.z + q.y * q2.q.w + q.z * q2.q.x - q.w * q2.q.y,
									q.x * q2.q.w - q.y * q2.q.z + q.z * q2.q.y + q.w * q2.q.x));
	return q_result;
}

Vector4f Quaternion::versor()
{
	return q;
}

void Quaternion::set_versor(Vector4f versor)
{
	q = versor;
}

void Quaternion::normalize()
{
	q = q.normalize();
}

Quaternion::~Quaternion()
{
}
