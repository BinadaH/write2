#include "toolbar.h"
#include "imgui.h"

ToolBar::ToolBar(AppStatus &status_) : status(status_){

}


ToolBar::~ToolBar(){

}

void ToolBar::Process(){
	ImGui::Begin("ToolBar", 0);
	ImGui::SliderFloat("Line Width", &status.curr_line_width, 1, 10);	

	ImGui::End();
}
