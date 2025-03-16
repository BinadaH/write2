#ifndef LINE_H
#define LINE_H

#include <vector>
#include "Vec2.h"
#include "imgui.h"
#include "camera.h"

class Line {
public:
	Line();
	~Line();

	void AddPoint(Vec2 point, Camera& cam);
	void AddPressPoint(float press);
	void Draw(ImDrawList* draw_list, Camera &cam);
	void ApplySmoothing();
	Vec2 GetLastSmoothedPoint();

	ImVec2 ApplyLoessFit(int start, int end, int current, float bandwidth);

public:
	float line_width;

private:
	std::vector<Vec2> points;
	std::vector<float> press;
	std::vector<ImVec2> draw_points;
	std::vector<Vec2> draw2_points;
};


#endif
