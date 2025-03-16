#include "app.h"


App::App(ImGuiIO &io_, AppStatus &stat_) : io(io_), status(stat_), canvas(stat_), toolbar(stat_){

}


App::~App(){

}



void App::Process(){
	canvas.Process(io);
	toolbar.Process();
}
