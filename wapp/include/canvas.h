#ifndef CANVAS_H
#define CANVAS_H
#include "imgui.h"
#include <vector>
#include "line.h"
class Canvas {
public:
	Canvas();
	~Canvas();

	void Process(ImGuiIO &io);

private:
	void handle_drawing(ImGuiIO& io);

private:
	Line* curr_line;
	std::vector<Line*> lines;
};

#endif