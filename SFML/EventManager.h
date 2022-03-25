#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <assert.h>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Utilities.h"
#include "GUI_Event.h"

enum class EventType {
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick,
	GUI_Click, GUI_Release, GUI_Hover, GUI_Leave
};

struct EventInfo {
	EventInfo() { code = 0; }
	EventInfo(int event) { code = event; }
	EventInfo(const GUI_Event& guiEvent) { gui = guiEvent; }
	union {
		int code;
		GUI_Event gui;
	};
};

struct EventDetails {
	EventDetails(const std::string& bindName)
		: name(bindName) {
		Clear();
	}
	std::string name;

	sf::Vector2i size;
	sf::Uint32 textEntered;
	sf::Vector2i mouse;
	int mouseWheelDelta;
	int keyCode; // Single key code.

	std::string guiInterface;
	std::string guiElement;
	GUI_EventType guiEvent;

	void Clear() {
		size = sf::Vector2i(0, 0);
		textEntered = 0;
		mouse = sf::Vector2i(0, 0);
		mouseWheelDelta = 0;
		keyCode = -1;
		guiInterface = "";
		guiElement = "";
		guiEvent = GUI_EventType::None;
	}
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding {
	Binding(const std::string& name) : name(name), details(name), c(0) {}
	~Binding() {
		// GUI portion.
		for (auto itr = events.begin();
			itr != events.end(); ++itr)
		{
			if (itr->first == EventType::GUI_Click || itr->first == EventType::GUI_Release ||
				itr->first == EventType::GUI_Hover || itr->first == EventType::GUI_Leave)
			{
				delete[] itr->second.gui.interface;
				delete[] itr->second.gui.element;
			}
		}
	}
	void BindEvent(EventType type, EventInfo info = EventInfo()) {
		events.emplace_back(type, info);
	}

	Events events;
	std::string name;
	int c; // Count of events that are "happening".

	EventDetails details;
};

using Bindings = std::unordered_map<std::string, Binding*>;
// Callback container.
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
// State callback container.
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager {
public:
	EventManager();
	~EventManager();

	bool AddBinding(Binding* binding);
	bool RemoveBinding(std::string name);

	void SetCurrentState(StateType state);
	void SetFocus(const bool& focus);

	// Needs to be defined in the header!
	template<class T>
	bool AddCallback(StateType state, const std::string& name,
		void(T::* func)(EventDetails*), T* instance)
	{
		auto itr = callbacks.emplace(state, CallbackContainer()).first;
		auto temp = std::bind(func, instance, std::placeholders::_1);
		return itr->second.emplace(name, temp).second;
	}

	bool RemoveCallback(StateType state, const std::string& name) {
		auto itr = callbacks.find(state);
		if (itr == callbacks.end()) { return false; }
		auto itr2 = itr->second.find(name);
		if (itr2 == itr->second.end()) { return false; }
		itr->second.erase(name);
		return true;
	}

	void HandleEvent(sf::Event& event);
	void HandleEvent(GUI_Event& event);
	void Update();

	// Getters.
	sf::Vector2i GetMousePos(sf::RenderWindow* wind = nullptr) {
		return (wind ? sf::Mouse::getPosition(*wind) : sf::Mouse::getPosition());
	}
private:
	void LoadBindings();

	StateType currentState;
	Bindings bindings;
	Callbacks callbacks;

	bool hasFocus;
};
