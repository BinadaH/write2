#include "canvas.h"
#include <iostream>

Canvas::Canvas() {
	curr_line = NULL;
}

Canvas::~Canvas() {
	for (Line* l : lines) {
		delete l;
	}

	lines.clear();
}

void Canvas::Process(ImGuiIO &io) {
	ImGui::Begin("Canvas", 0);
	

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->Flags |= ImDrawListFlags_AntiAliasedLines;
	draw_list->AddCircle(io.MousePos, 2, IM_COL32(255, 0, 0, 255));

	//std::cout << io.MousePos.x << "; " << io.MousePos.y << "\n";

	
	if (curr_line) {
		curr_line->Draw(draw_list);
		
	}
	
	for (Line* l : lines) {
		l->Draw(draw_list);
	}

	handle_drawing(io);


	ImGui::End();
}

void Canvas::handle_drawing(ImGuiIO& io)
{
	bool mouse_down = ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered();
	
	static float delta = 0;

	if (mouse_down) {
		if (curr_line) {
			//update curr_line
			delta += io.DeltaTime;
			if (delta > 0.001) {
				std::cout << "UpdateLine\n";
				curr_line->AddPoint(Vec2(io.MousePos.x, io.MousePos.y));
				curr_line->UpdateWithExpMovingAverage();
				delta = 0;
			}
			
		}
		else {
			//create curr_line
			std::cout << "New Line\n";
			curr_line = new Line();
		}
	}
	else {
		if (curr_line) {
			//push curr_line
			lines.push_back(curr_line);
			curr_line = NULL;
		}
	}
}
