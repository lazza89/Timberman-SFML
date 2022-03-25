#pragma once
#include <string>

enum class GUI_EventType{ None, Click, Release, Hover, Leave };

struct ClickCoordinates{
	float x, y;
};

struct GUI_Event{
	GUI_EventType type;
	const char* element;
	const char* interface;
	union{
		ClickCoordinates clickCoords;
	};
};