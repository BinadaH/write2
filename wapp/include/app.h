#ifndef APP_H
#define APP_H

#include "imgui.h"
#include "app_status.h"
#include "canvas.h"
#include "toolbar.h"

class App{
public:
	App(ImGuiIO &io_, AppStatus &stat_);
	~App();

	void Process();

private:
	ImGuiIO &io;
	AppStatus &status;

	Canvas canvas;
	ToolBar toolbar;

};




#endif
