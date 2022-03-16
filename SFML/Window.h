#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "EventManager.h"

class Window{
public:
	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	void BeginDraw();
	void EndDraw();

	void Update();

	bool IsDone();
	bool IsFullscreen();
	bool IsFocused();

	void ToggleFullscreen(EventDetails* details);
	void Close(EventDetails* details = nullptr);

	sf::RenderWindow* GetRenderWindow();
	EventManager* GetEventManager();
	sf::Vector2u GetWindowSize();
	sf::FloatRect GetViewSpace();
private:
	void Setup(const std::string& title, const sf::Vector2u& size);
	void Create();

	sf::RenderWindow window;
	EventManager eventManager;
	sf::Vector2u windowSize;
	std::string windowTitle;
	bool isDone;
	bool isFullscreen;
	bool isFocused;
};