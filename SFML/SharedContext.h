#pragma once
#include "Window.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "AudioManager.h"
#include "SoundManager.h"
#include "System_Manager.h"
#include "Entity_Manager.h"
#include "DebugOverlay.h"
#include <TGUI/TGUI.hpp>

struct SharedContext {
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr),
		fontManager(nullptr),
		audioManager(nullptr),
		soundManager(nullptr),
		entityManager(nullptr),
		gameMap(nullptr)
	{}

	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
	FontManager* fontManager;
	AudioManager* audioManager;
	SoundManager* soundManager;
	EntityManager* entityManager;
	Map* gameMap;
	DebugOverlay debugOverlay;
};