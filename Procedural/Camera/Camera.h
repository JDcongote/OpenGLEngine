#pragma once
#define TRANSLATE_FORWARD 0
#define TRANSLATE_BACKWARDS 1
#define TRANSLATE_RIGHT 2
#define TRANSLATE_LEFT 3

#include "../CommonHeaders.h";

#include "../Math/Quaternion/Quaternion.h"

#include <stdio.h>
#include <stdlib.h>

class Camera
{
public:
	Camera();
	Camera(const Vector position, const float near, const float far, const float fov, const WindowDimensions win);

	void set_ubo();

	Vector3f get_position();

	Matrix4f get_view_matrix();
	Matrix4f get_projection_matrix();
	Matrix4f get_inverse_view_matrix();
	Matrix4f get_inverse_projection_matrix();

	void lookAt(const Vector position, const Vector target_pos, const Vector up);
	void rotate(const float x, const float y, const float z);
	void rotate_y(const float y);
	void rotate_x(const float x);
	void translate(const int dir, const float z);
	void update_pitch_yaw(const float pitch, const float yaw);
	void update();

	const float get_viewport_width() const;
	const float get_viewport_height() const;

	bool should_update;
	bool should_move_camera;

	GLuint ubo_blockid = 0;
	GLuint camera_ubo = 0;

	float* camera_ubo_ptr;
	~Camera();

private:
	Matrix4f view_matrix;	
	Matrix4f projection_matrix;
	Matrix4f projection_matrix_inv;

	Matrix4f rotation_matrix;
	Matrix4f translation_mat;

	Vector3f forwards;
	Vector3f right;
	Vector3f up;

	Quaternion q_pitch;
	Quaternion q_yaw;
	Quaternion q_orientation;

	WindowDimensions window_dimensions;

	float current_yaw = 0.0f;
	float current_pitch = 0.0f;
	float speed = 0.05f;
	float rotation_speed = 0.05f;	
};

