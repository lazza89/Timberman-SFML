#pragma once
#include "Window.h"
#include "EventManager.h"
struct SharedContext{
	SharedContext()
		:wind(nullptr),
		eventManager(nullptr){}
	Window* wind;
	EventManager* eventManager;
};