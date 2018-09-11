#include "Matrix4f.h"


/**
	Create an identity matrix
*/
Matrix4f::Matrix4f() : Matrix4f(0)
{
	mat[0] = 1.0f;
	mat[5] = 1.0f;
	mat[10] = 1.0f;
	mat[15] = 1.0f;
	m11 = mat[0];
	m22 = mat[5];
	m33 = mat[10];
	m44 = mat[15];
	
}
Matrix4f::Matrix4f(int zeros)
{	
	for (int i = 0; i < size_t; i++)
	{
		mat[i] = 0.0f;
	}
	m11 = mat[0];
	m12 = mat[1];
	m13 = mat[2];
	m14 = mat[3];
	m21 = mat[4];
	m22 = mat[5];
	m23 = mat[6];
	m24 = mat[7];
	m31 = mat[8];
	m32 = mat[9];
	m33 = mat[10];
	m34 = mat[11];
	m41 = mat[12];
	m42 = mat[13];
	m43 = mat[14];
	m44 = mat[15];
}

Matrix4f::Matrix4f(const Vector4f col0, const Vector4f col1, const Vector4f col2, const Vector4f col3)
{
	m11 = col0.x;
	m12 = col0.y;
	m13 = col0.z;
	m14 = col0.w;

	m21 = col1.x;
	m22 = col1.y;
	m23 = col1.z;
	m24 = col1.w;

	m31 = col2.x;
	m32 = col2.y;
	m33 = col2.z;
	m34 = col2.w;

	m41 = col3.x;
	m42 = col3.y;
	m43 = col3.z;
	m44 = col3.w;

	_update();
}

float& Matrix4f::operator [] (const int index) {
	return mat[index];
}

Matrix4f Matrix4f::operator * (Matrix4f& right)
{
	Matrix4f result = Matrix4f(0);

	int indices[] = {
		0,  1,  2,  3, //first column
		4,  5,  6,  7, //second
		8,  9,  10, 11, //third
		12, 13, 14, 15 //fourth column
	};

	int r_index = 0;
	for (int col = 0; col < 4; col++) {
		for (int row = 0; row < 4; row++) {
			float sum = 0.0f;
			for (int i = 0; i < 4; i++) {
				sum += right.mat[i + col * 4] * mat[row + i * 4];
			}
			result.mat[r_index] = sum;
			r_index++;
		}
	}


	/*int i, j, k;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			result[j * 4 + i] = 0;

			for (k = 0; k < 4; k++) {
				result[j * 4 + i] += mat[k * 4 + i] * right[j * 4 + k];
			}
		}
	}*/

	/*for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			float sum = 0.0;
			for (int k = 0; k < 4; k++)
				sum = sum + mat[i * 4 + k] * right[k * 4 + j];
			result[i * 4 + j] = sum;
		}

	}*/
	result.m11 = result[0];
	result.m12 = result[1];
	result.m13 = result[2];
	result.m14 = result[3];
	result.m21 = result[4];
	result.m22 = result[5];
	result.m23 = result[6];
	result.m24 = result[7];
	result.m31 = result[8];
	result.m32 = result[9];
	result.m33 = result[10];
	result.m34 = result[11];
	result.m41 = result[12];
	result.m42 = result[13];
	result.m43 = result[14];
	result.m44 = result[15];
	return result;
}

Vector4f Matrix4f::operator*(Vector4f &v)
{
	return Vector4f(mat[0] * v.x + mat[4] * v.y + mat[8] * v.z + mat[12] * v.w, mat[1] * v.x + mat[5] * v.y + mat[9] * v.z + mat[13] * v.w, 
		mat[2] * v.x + mat[6] * v.y + mat[10] * v.z + mat[14] * v.w, mat[3] * v.x + mat[7] * v.y + mat[11] * v.z + mat[15] * v.w);
}

float Matrix4f::get(int index) const{
	if (index < size_t) {
		return mat[index];
	}
	else {
		return NULL;
	}
}

float* Matrix4f::get_as_array()
{
	_update();
	return mat;
}

Matrix4f Matrix4f::inverse() const
{
	float det = determinant();
	Matrix4f mat_inverse = Matrix4f();
	/* there is no inverse if determinant is zero (not likely unless scale is
	broken) */
	if (0.0f == det) {
		//fprintf(stderr, "WARNING. matrix has no determinant. can not invert\n");
		return *this;
	}
	float inv_det = 1.0f / det;

	mat_inverse.m11 = inv_det * (mat[9] * mat[14] * mat[7] - mat[13] * mat[10] * mat[7] +
		mat[13] * mat[6] * mat[11] - mat[5] * mat[14] * mat[11] -
		mat[9] * mat[6] * mat[15] + mat[5] * mat[10] * mat[15]);
	mat_inverse.m12 = inv_det * (mat[13] * mat[10] * mat[3] - mat[9] * mat[14] * mat[3] -
		mat[13] * mat[2] * mat[11] + mat[1] * mat[14] * mat[11] +
		mat[9] * mat[2] * mat[15] - mat[1] * mat[10] * mat[15]);
	mat_inverse.m13 = inv_det * (mat[5] * mat[14] * mat[3] - mat[13] * mat[6] * mat[3] +
		mat[13] * mat[2] * mat[7] - mat[1] * mat[14] * mat[7] -
		mat[5] * mat[2] * mat[15] + mat[1] * mat[6] * mat[15]);
	mat_inverse.m14 = inv_det * (mat[9] * mat[6] * mat[3] - mat[5] * mat[10] * mat[3] -
		mat[9] * mat[2] * mat[7] + mat[1] * mat[10] * mat[7] +
		mat[5] * mat[2] * mat[11] - mat[1] * mat[6] * mat[11]);
	mat_inverse.m21 = inv_det * (mat[12] * mat[10] * mat[7] - mat[8] * mat[14] * mat[7] -
		mat[12] * mat[6] * mat[11] + mat[4] * mat[14] * mat[11] +
		mat[8] * mat[6] * mat[15] - mat[4] * mat[10] * mat[15]);
	mat_inverse.m22 = inv_det * (mat[8] * mat[14] * mat[3] - mat[12] * mat[10] * mat[3] +
		mat[12] * mat[2] * mat[11] - mat[0] * mat[14] * mat[11] -
		mat[8] * mat[2] * mat[15] + mat[0] * mat[10] * mat[15]);
	mat_inverse.m23 = inv_det * (mat[12] * mat[6] * mat[3] - mat[4] * mat[14] * mat[3] -
		mat[12] * mat[2] * mat[7] + mat[0] * mat[14] * mat[7] +
		mat[4] * mat[2] * mat[15] - mat[0] * mat[6] * mat[15]);
	mat_inverse.m24 = inv_det * (mat[4] * mat[10] * mat[3] - mat[8] * mat[6] * mat[3] +
		mat[8] * mat[2] * mat[7] - mat[0] * mat[10] * mat[7] -
		mat[4] * mat[2] * mat[11] + mat[0] * mat[6] * mat[11]);
	mat_inverse.m31 = inv_det * (mat[8] * mat[13] * mat[7] - mat[12] * mat[9] * mat[7] +
		mat[12] * mat[5] * mat[11] - mat[4] * mat[13] * mat[11] -
		mat[8] * mat[5] * mat[15] + mat[4] * mat[9] * mat[15]);
	mat_inverse.m32 = inv_det * (mat[12] * mat[9] * mat[3] - mat[8] * mat[13] * mat[3] -
		mat[12] * mat[1] * mat[11] + mat[0] * mat[13] * mat[11] +
		mat[8] * mat[1] * mat[15] - mat[0] * mat[9] * mat[15]);
	mat_inverse.m33 = inv_det * (mat[4] * mat[13] * mat[3] - mat[12] * mat[5] * mat[3] +
		mat[12] * mat[1] * mat[7] - mat[0] * mat[13] * mat[7] -
		mat[4] * mat[1] * mat[15] + mat[0] * mat[5] * mat[15]);
	mat_inverse.m34 = inv_det * (mat[8] * mat[5] * mat[3] - mat[4] * mat[9] * mat[3] -
		mat[8] * mat[1] * mat[7] + mat[0] * mat[9] * mat[7] +
		mat[4] * mat[1] * mat[11] - mat[0] * mat[5] * mat[11]);
	mat_inverse.m41 = inv_det * (mat[12] * mat[9] * mat[6] - mat[8] * mat[13] * mat[6] -
		mat[12] * mat[5] * mat[10] + mat[4] * mat[13] * mat[10] +
		mat[8] * mat[5] * mat[14] - mat[4] * mat[9] * mat[14]);
	mat_inverse.m42 = inv_det * (mat[8] * mat[13] * mat[2] - mat[12] * mat[9] * mat[2] +
		mat[12] * mat[1] * mat[10] - mat[0] * mat[13] * mat[10] -
		mat[8] * mat[1] * mat[14] + mat[0] * mat[9] * mat[14]);
	mat_inverse.m43 = inv_det * (mat[12] * mat[5] * mat[2] - mat[4] * mat[13] * mat[2] -
		mat[12] * mat[1] * mat[6] + mat[0] * mat[13] * mat[6] +
		mat[4] * mat[1] * mat[14] - mat[0] * mat[5] * mat[14]);
	mat_inverse.m44 = inv_det * (mat[4] * mat[9] * mat[2] - mat[8] * mat[5] * mat[2] +
		mat[8] * mat[1] * mat[6] - mat[0] * mat[9] * mat[6] -
		mat[4] * mat[1] * mat[10] + mat[0] * mat[5] * mat[10]);

	mat_inverse._update();
	return mat_inverse;
}


float Matrix4f::determinant() const
{
	return mat[12] * mat[9] * mat[6] * mat[3] -
		mat[8] * mat[13] * mat[6] * mat[3] -
		mat[12] * mat[5] * mat[10] * mat[3] +
		mat[4] * mat[13] * mat[10] * mat[3] +
		mat[8] * mat[5] * mat[14] * mat[3] -
		mat[4] * mat[9] * mat[14] * mat[3] -
		mat[12] * mat[9] * mat[2] * mat[7] +
		mat[8] * mat[13] * mat[2] * mat[7] +
		mat[12] * mat[1] * mat[10] * mat[7] -
		mat[0] * mat[13] * mat[10] * mat[7] -
		mat[8] * mat[1] * mat[14] * mat[7] +
		mat[0] * mat[9] * mat[14] * mat[7] +
		mat[12] * mat[5] * mat[2] * mat[11] -
		mat[4] * mat[13] * mat[2] * mat[11] -
		mat[12] * mat[1] * mat[6] * mat[11] +
		mat[0] * mat[13] * mat[6] * mat[11] +
		mat[4] * mat[1] * mat[14] * mat[11] -
		mat[0] * mat[5] * mat[14] * mat[11] -
		mat[8] * mat[5] * mat[2] * mat[15] +
		mat[4] * mat[9] * mat[2] * mat[15] +
		mat[8] * mat[1] * mat[6] * mat[15] -
		mat[0] * mat[9] * mat[6] * mat[15] -
		mat[4] * mat[1] * mat[10] * mat[15] +
		mat[0] * mat[5] * mat[10] * mat[15];
}

void Matrix4f::translate(const float x, const float y, const float z)
{
	m41 += x;
	m42 += y;
	m43 += z;
	_update();
}

void Matrix4f::translate_x(const float x)
{
}

void Matrix4f::translate_y(const float y)
{
}

void Matrix4f::translate_z(const float z)
{
	m43 += z;
	_update();
}

void Matrix4f::scale(const float x, const float y, const float z)
{
	m11 += x;
	m22 += y;
	m33 += z;
	_update();
}

void Matrix4f::rotate(const float x, const float y, const float z)
{


	float cos_x = cosf(x * ONE_DEG_IN_RAD);
	float cos_y = cosf(y * ONE_DEG_IN_RAD);
	float cos_z = cosf(z * ONE_DEG_IN_RAD);

	float sin_x = sinf(x * ONE_DEG_IN_RAD);
	float sin_y = sinf(y * ONE_DEG_IN_RAD);
	float sin_z = sinf(z * ONE_DEG_IN_RAD);

	m11 += cos_y * cos_z;
	m12 += cos_z * sin_x*sin_y + cos_x * sin_z;
	m13 += sin_x * sin_z - cos_x * cos_z*sin_y;
	m21 += -cos_y * sin_z;
	m22 += cos_x * cos_z - sin_x * sin_y*sin_z;
	m23 += cos_z * sin_x + cos_x * sin_y*sin_z;
	m31 += sin_y;
	m32 += -cos_y * sin_x;
	m33 += cos_x * cos_y;
	_update();
}

void Matrix4f::rotate_x(const float x)
{
	Matrix4f new_mat = Matrix4f();

	float rad = x * ONE_DEG_IN_RAD;
	const float cosx = cosf(rad);
	const float sinx = sinf(rad);

	new_mat.m22 = cosx;
	new_mat.m23 = sinx;
	new_mat.m32 = -sinx;
	new_mat.m33 = cosx;
	new_mat._update();

	*this = new_mat * *this;

}

void Matrix4f::rotate_y(const float y)
{
	Matrix4f new_mat = Matrix4f();

	float rad = y * ONE_DEG_IN_RAD;
	const float cosx = cosf(rad);
	const float sinx = sinf(rad);

	new_mat.m11 = cosx;
	new_mat.m13 = -sinx;
	new_mat.m31 = sinx;
	new_mat.m33 = cosx;
	new_mat._update();

	*this = new_mat * *this;
}

void Matrix4f::rotate_z(const float z)
{
	const float cosx = cosf(z);
	const float sinx = sinf(z);
}

void Matrix4f::set_position(const float x, const float y, const float z)
{
	m41 = x;
	m42 = y;
	m43 = z;
	_update();
}

void Matrix4f::set_scale(const float x, const float y, const float z)
{
	m11 = x;
	m22 = y;
	m33 = z;
	_update();
}

void Matrix4f::set_rotation(const float x, const float y, const float z)
{
	float cos_x = cosf(x);
	float cos_y = cosf(y);
	float cos_z = cosf(z);

	float sin_x = sinf(x);
	float sin_y = sinf(y);
	float sin_z = sinf(z);

	m11 = cos_y*cos_z;
	m12 = cos_z *sin_x*sin_y + cos_x*sin_z;
	m13 = sin_x*sin_z - cos_x*cos_z*sin_y;
	m21 = -cos_y*sin_z;
	m22 = cos_x*cos_z - sin_x*sin_y*sin_z;
	m23 = cos_z*sin_x + cos_x*sin_y*sin_z;
	m31 = sin_y;
	m32 = -cos_y*sin_x;
	m33 = cos_x*cos_y;

	_update();
}

void Matrix4f::_update() {
	
	mat[0] = m11;
	mat[1] = m12;
	mat[2] = m13;
	mat[3] = m14;
	mat[4] = m21;
	mat[5] = m22;
	mat[6] = m23;
	mat[7] = m24;
	mat[8] = m31;
	mat[9] = m32;
	mat[10] = m33;
	mat[11] = m34;
	mat[12] = m41;
	mat[13] = m42;
	mat[14] = m43;
	mat[15] = m44;

}

// Destructor 
Matrix4f::~Matrix4f()
{
}