#ifndef APPSTATUS_H
#define APPSTATUS_H

enum  WAPPTOOL{
	PEN,
	HAND,
	SELECT
};

class AppStatus{
public:
	AppStatus();
        void SetCurrPress(float press);
	
	float curr_line_width;
	float curr_press;
	enum WAPPTOOL curr_tool;
	


	
};


#endif
