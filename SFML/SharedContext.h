#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
struct SharedContext {
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr)
	{}

	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
};