#ifndef TOOLBAR_H
#define TOOLBAR_H
#include "app_status.h"

class ToolBar{
	public:
		ToolBar(AppStatus &status);
		~ToolBar();
		void Process();
	private:
		AppStatus &status;
};

#endif
