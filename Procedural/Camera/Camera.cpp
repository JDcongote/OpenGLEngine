#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(const Vector position, const float near, const float far, const float fov, const float width, const float height)
{

	//Initialize Translation matrix
	translation_mat = Matrix4f();
	//projection_matrix = Matrix4f();
	rotation_matrix = Matrix4f();		
	view_matrix = Matrix4f();

	// Quaternion stuff
	q_pitch = Quaternion(current_pitch, 1.0f, 0.0f, 0.0f);
	q_yaw = Quaternion(current_yaw, 0.0f, 1.0f, 0.0f);
	q_orientation = q_pitch * q_yaw;
	q_orientation.normalize();
	rotation_matrix = q_orientation.get_matrix();
	
	translation_mat.set_position(-position.x,-position.y,-position.z);

	float range = tan((fov * ONE_DEG_IN_RAD) * 0.5f) * near;
	this->width = width;
	this->height = height;
	float aspect = width / height;
	projection_matrix.m11 = (2.0f * near) / (range * aspect + range * aspect);;
	projection_matrix.m22 = near / range;
	projection_matrix.m33 = -(far + near) / (far - near);
	projection_matrix.m34 = -1.0f;
	projection_matrix.m43 = -(2.0f * far * near) / (far - near);
	projection_matrix.m44 = 0;
	// Needed for raycast.
	projection_matrix_inv = projection_matrix.inverse();

	view_matrix = rotation_matrix * translation_mat;

	should_update = true;
	should_move_camera = false;
}

Matrix4f Camera::get_view_matrix()
{
	return view_matrix;
}

Matrix4f Camera::get_projection_matrix()
{
	return projection_matrix;
}

Matrix4f Camera::get_inverse_view_matrix()
{
	// View matrix inverse needs to be generated often
	should_update = true;
	update();
	Matrix4f inv_view = view_matrix.inverse();
	should_update = false;
	return inv_view;
}

Matrix4f Camera::get_inverse_projection_matrix()
{
	// Projection matrix doesnt need to be updated as much
	return projection_matrix_inv;
}

void Camera::lookAt(const Vector pos, const Vector target_pos, const Vector up)
{
	/*cam_position = pos;

	translation_mat.m41 = -*pos.x;
	translation_mat.m42 = -*pos.x;
	translation_mat.m43 = -*pos.x;

	distance_to_target = Vector3f(target_pos.x - pos.x, target_pos.y - pos.y, target_pos.z - pos.z);
	forwards = distance_to_target.normalize();
	right = forwards.cross(up);
	this->up = right.cross(forwards);

	rotation_matrix.m11 = *right.x;
	rotation_matrix.m21 = *right.y;
	rotation_matrix.m31 = *right.z;

	rotation_matrix.m12 = *up.x;
	rotation_matrix.m22 = *up.y;
	rotation_matrix.m32 = *up.z;

	rotation_matrix.m13 = -*forwards.x;
	rotation_matrix.m23 = -*forwards.y;
	rotation_matrix.m33 = -*forwards.z;

	view_matrix = rotation_matrix * translation_mat;
	should_update = true;*/
}

void Camera::rotate(const float x, const float y, const float z)
{
	rotation_matrix.rotate(x, y, z);
}

void Camera::rotate_y(const float y)
{
	current_yaw += (y*rotation_speed);
}

void Camera::rotate_x(const float x)
{
	current_pitch += (x*rotation_speed);
}

void Camera::translate(const int dir, const float z)
{
	float dx = 0; //how much we strafe on x
	float dz = 0; //how much we walk on z
	switch (dir)
	{
	case TRANSLATE_FORWARD:
	{
		dz = 2;
		break;
	}

	case TRANSLATE_BACKWARDS:
	{
		dz = -2;
		break;
	}	
	case TRANSLATE_LEFT:
	{
		dx = -2;
		break;
	}

	case TRANSLATE_RIGHT:
	{
		dx = 2;
		break;
	}
	default:
		break;
	}
	
	Vector3f translation = (forwards * -dz + right * dx) * speed;

	translation_mat.translate(-translation.x, -translation.y, -translation.z);
}

void Camera::update_pitch_yaw(const float pitch, const float yaw)
{
	current_pitch += pitch;
	current_yaw += yaw;
}

void Camera::update()
{
	if (should_update) {

		q_pitch = Quaternion(current_pitch, 1.0f, 0.0f, 0.0f);
		q_yaw = Quaternion(current_yaw, 0.0f, 1.0f, 0.0f);

		q_orientation = q_pitch * q_yaw;
		q_orientation.normalize();
		rotation_matrix = q_orientation.get_matrix();
		view_matrix = rotation_matrix * translation_mat;

		forwards = Vector3f(view_matrix.m13, view_matrix.m23, view_matrix.m33);
		right = Vector3f(view_matrix.m11, view_matrix.m21, view_matrix.m31);
		up = Vector3f(view_matrix.m12, view_matrix.m22, view_matrix.m32);
	}
}

const float Camera::get_viewport_width() const
{
	return width;
}

const float Camera::get_viewport_height() const
{
	return height;
}


Camera::~Camera()
{
}
