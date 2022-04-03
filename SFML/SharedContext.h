#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include <TGUI/TGUI.hpp>

struct SharedContext {
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr),
		fontManager(nullptr)
	{}

	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
	FontManager* fontManager;
};