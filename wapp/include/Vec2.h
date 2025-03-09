#ifndef VEC2_H
#define VEC2_H

#include "imgui.h"

class Vec2 {
public:
	Vec2(float x_ = 0.f, float y_ = 0.f) {
		x = x_;
		y = y_;
	}

	Vec2(ImVec2 vec) {
		x = vec.x;
		y = vec.y;
	}

	Vec2 operator +(Vec2 b) {
		return Vec2(x + b.x, y + b.y);
	}
	Vec2 operator -(Vec2 b) {
		return Vec2(x - b.x, y - b.y);
	}
	Vec2 operator *(Vec2 b) {
		return Vec2(x * b.x, y * b.y);
	}
	Vec2 operator /(Vec2 b) {
		return Vec2(x / b.x, y / b.y);
	}

	Vec2 operator +(float b) {
		return Vec2(x + b, y + b);
	}
	Vec2 operator -(float b) {
		return Vec2(x - b, y - b);
	}
	Vec2 operator *(float b) {
		return Vec2(x * b, y * b);
	}
	Vec2 operator /(float b) {
		return Vec2(x / b, y / b);
	}


	void operator +=(Vec2 b) {
		x += b.x;
		y += b.y;
	}
	void operator -=(Vec2 b) {
		x -= b.x;
		y -= b.y;
	}
	void operator *=(Vec2 b) {
		x *= b.x;
		y *= b.y;
	}
	void operator /=(Vec2 b) {
		x /= b.x;
		y /= b.y;
	}

	void operator +=(float b) {
		x += b;
		y += b;
	}
	void operator -=(float b) {
		x -= b;
		y -= b;
	}
	void operator *=(float b) {
		x *= b;
		y *= b;
	}
	void operator /=(float b) {
		x /= b;
		y /= b;
	}
	float x, y;

	
	ImVec2 toImVec2() {
		return ImVec2(x, y);
	}


	float length() {
		return sqrtf(x * x + y * y);
	}

};




#endif