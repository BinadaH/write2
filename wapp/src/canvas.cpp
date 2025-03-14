#include "canvas.h"
#include <iostream>

Canvas::Canvas() {
	curr_line = NULL;
	curr_press = 1;
	cam.zoom_vel = 0.4;
}

Canvas::~Canvas() {
	for (Line* l : lines) {
		delete l;
	}

	lines.clear();
}

void Canvas::Process(ImGuiIO &io) {
	ImGui::Begin("Canvas", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);


	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->Flags |= ImDrawListFlags_AntiAliasedLines;


	ImVec2 canvas_sz = ImGui::GetWindowSize();
	ImVec2 mouse_pos = io.MousePos;
	ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();
	ImVec2 canvas_p1 = ((Vec2)canvas_p0 + canvas_sz).im();


	cam.window_pos = canvas_p0;
	cam.cam_size = canvas_sz;

	ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	const bool is_hovered = ImGui::IsItemHovered(); // Hovered
	const bool is_active = ImGui::IsItemActive();   // Held


	if (is_hovered) {
		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		if (io.MouseDown[2]) {
			Vec2 move_to = (Vec2)io.MouseDelta * -1;
			cam.MoveBy(move_to);
		}
		if (io.MouseWheel) {
			cam.Zoom(io.MouseWheel);
		}
	}

	//std::cout << cam.cam_size << "\n";
	Vec2 circ_pos = Vec2(0, 0);
	draw_list->AddCircleFilled(cam.WorldToScreen(circ_pos).im(), cam.ScaleWorldToScreen(25), IM_COL32(255, 0, 0, 255));

	//std::cout << io.MousePos.x << "; " << io.MousePos.y << "\n";

#define GRID_COL IM_COL32(100, 100, 100, 255)

	//Draw Grid
	static const int grid_size = 100;
	Vec2 start_i = (cam.world_pos - cam.cam_size) / grid_size;
	for (int x = (int)(start_i.x) * grid_size; x < cam.world_pos.x + cam.cam_size.x; x += grid_size) {
		Vec2 pos = Vec2(x, 0);
		ImVec2 p0 = cam.WorldToScreen(pos).im();
		ImVec2 p1 = p0;
		p0.y = 0;
		p1.y = canvas_sz.y;
		draw_list->AddLine(p0, p1, GRID_COL, 1);
	}
	for (int y = (int)(start_i.y) * grid_size; y < cam.world_pos.y + cam.cam_size.y; y += grid_size) {
		Vec2 pos = Vec2(0, y);
		ImVec2 p0 = cam.WorldToScreen(pos).im();
		ImVec2 p1 = p0;
		p0.x = 0;
		p1.x = canvas_sz.x;
		draw_list->AddLine(p0, p1, GRID_COL, 1);
	}


	draw_list->PushClipRect(canvas_p0, canvas_p1, true);
	if (curr_line) {
		curr_line->Draw(draw_list, cam);
	}
	draw_list->AddCircle(io.MousePos, 2, IM_COL32(255, 0, 0, 255));

	for (Line* l : lines) {
		l->Draw(draw_list, cam);
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
				curr_line->AddPoint(mouse_pos, cam);
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
