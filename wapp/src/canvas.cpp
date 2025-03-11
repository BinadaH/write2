#include "canvas.h"
#include <iostream>

Canvas::Canvas() {
	curr_line = NULL;
	curr_press = 1;
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
	

	ImVec2 canvas_sz = ImGui::GetWindowSize();
	ImVec2 mouse_pos = io.MousePos;
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_p1 = ((Vec2)canvas_p0 + canvas_sz).im();


	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held


	if (is_hovered) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
	}
	


	//std::cout << io.MousePos.x << "; " << io.MousePos.y << "\n";

	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (curr_line) {
		curr_line->Draw(draw_list);
	}
	draw_list->AddCircle(io.MousePos, 2, IM_COL32(255, 0, 0, 255));
	
	for (Line* l : lines) {
		l->Draw(draw_list);
	}


	handle_drawing(io.DeltaTime, mouse_pos, is_hovered, is_active);
	draw_list->PopClipRect();

	ImGui::End();
}

void Canvas::handle_drawing(float &dt, ImVec2 &mouse_pos, const bool &is_hovered, const bool &is_active)
{
	static float delta = 0;
	static ImVec2 last_pos;
	if (is_hovered && is_active) {
		if (curr_line) {
			//update curr_line
			delta += dt;
			if (((Vec2)last_pos - mouse_pos).length() >2) {
				//std::cout << "UpdateLine\n";
				curr_line->AddPoint(mouse_pos);
				curr_line->AddPressPoint(curr_press);
				curr_line->ApplySmoothing();
				last_pos = mouse_pos;
				delta = 0;
			}
			
		}
		else {
			//create curr_line
			//std::cout << "New Line\n";
			last_pos = mouse_pos;
			curr_line = new Line();
		}
	}
	else {
		if (curr_line) {
			//push curr_line
			//std::cout << "End Line\n";
			lines.push_back(curr_line);
			curr_line = NULL;
		}
	}
}

void Canvas::SetCurrPress(float press)
{
	curr_press = press;
}

void Canvas::SetCurrPos(int x, int y)
{
	curr_pos.x = x;
	curr_pos.y = y;
}
