#include "Raycaster.h"




Raycaster::Raycaster(Camera camera)
{
	cam = camera;
}

void Raycaster::update_coords_view(const double x, const double y)
{
	mouse_x = x;
	mouse_y = y;

	mouse_x = (2.0f * mouse_x) / cam.get_viewport_width() - 1.0f;
	mouse_y = 1.0f - (2.0f * mouse_y) / cam.get_viewport_height();

	ray = Vector4f(mouse_x, mouse_y, -1.0f, 1.0f);
}

const Vector3f Raycaster::get_ray_world() const
{
	return ray_world;
}

void Raycaster::raycast()
{
	Vector4f ray_eye = cam.get_inverse_projection_matrix() * ray;
	ray_eye = Vector4f(ray_eye.x, ray_eye.y, -1.0f, 0.0f);


	Vector4f ray_world_unormalized = cam.get_inverse_view_matrix() * ray_eye;
	ray_world_unormalized = ray_world_unormalized.normalize();
	ray_world = Vector3f(ray_world_unormalized.x, ray_world_unormalized.y, ray_world_unormalized.z);
}

Raycaster::~Raycaster()
{
}
