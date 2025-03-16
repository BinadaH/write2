#ifndef CANVAS_H
#define CANVAS_H
#include "imgui.h"
#include <vector>
#include "line.h"
#include "camera.h"
#include "app_status.h"

class Canvas {
public:
	Canvas(AppStatus &stat);
	~Canvas();

	void Process(ImGuiIO &io);

	void handle_drawing(float& dt, ImVec2& mouse_pos, const bool& is_hovered, const bool& is_active);
	void SetCurrPress(float press);
	void SetCurrPos(int x, int y);

private:
	AppStatus &status;
	Line* curr_line;
	float curr_press;
	Vec2 curr_pos;
	std::vector<Line*> lines;
	Camera cam;
};

#endif
