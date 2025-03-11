#ifndef CAMERA_H
#define CAMERA_H
#include "Vec2.h"

class Camera {
public:
	Camera();
	~Camera();

	void MoveTo(Vec2 &world_target);
	void MoveBy(Vec2& screen_relative);
	void SetZoom(float zoom);
	void Zoom(int zoom_f);
	Vec2 ScreenToWorld(Vec2 &screen);
	Vec2 WorldToScreen(Vec2 &world);
	float ScaleWorldToScreen(float size);


public:
	float zoom;
	Vec2 world_pos;
	float zoom_vel;
	Vec2 cam_size;
	Vec2 window_pos;
};



#endif