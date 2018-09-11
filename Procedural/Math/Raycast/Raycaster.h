#pragma once
#include "../../Camera/Camera.h"
class Raycaster
{
public:
	Raycaster();
	Raycaster(Camera camera);
	void update_coords_view(const double x, const double y);
	const Vector3f get_ray_world() const;
	void raycast();

	~Raycaster();
private:
	// The camera to use this raycaster from
	Camera cam;

	// The current mouse coords
	double mouse_x, mouse_y;

	// the Ray vector
	Vector4f ray;
	Vector3f ray_world;
};

