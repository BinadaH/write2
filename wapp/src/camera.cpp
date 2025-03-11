#include "camera.h"

Camera::Camera()
{
	zoom = 1;
	world_pos.x = 0;
	world_pos.y = 0;
	zoom_vel = 1;
	window_pos.x = 0;
	window_pos.y = 0;
}

Camera::~Camera()
{

}

void Camera::MoveTo(Vec2 &world_target)
{
	world_pos.x = world_target.x;
	world_pos.y = world_target.y;
}

void Camera::MoveBy(Vec2& screen_rel)
{
	world_pos.x += screen_rel.x / zoom;
	world_pos.y += screen_rel.y / zoom;
}

void Camera::SetZoom(float _zoom)
{
	zoom = _zoom;
}

void Camera::Zoom(int zoom_f)
{
	zoom = std::min(std::max(0.5f, zoom + (float)zoom_f * zoom_vel), 5.f);
}

Vec2 Camera::ScreenToWorld(Vec2 &screen)
{
	return (screen - window_pos - cam_size / 2) / zoom + world_pos;
}

Vec2 Camera::WorldToScreen(Vec2 &world)
{
	return (world - world_pos) * zoom + cam_size / 2 + window_pos;
}

float Camera::ScaleWorldToScreen(float size)
{
	return size * zoom;
}




