#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "GUI_Manager.h"

struct SharedContext {
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr),
		fontManager(nullptr),
		guiManager(nullptr)
	{}

	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
	FontManager* fontManager;
	GUI_Manager* guiManager;
};