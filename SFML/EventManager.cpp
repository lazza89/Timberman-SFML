#include "EventManager.h"
#include "StateManager.h"
#include <cstring>

EventManager::EventManager()
	:currentState(StateType(0)), hasFocus(true)
{
	LoadBindings();
}

EventManager::~EventManager() {
	for (auto& itr : bindings) {
		delete itr.second;
	}
}

bool EventManager::AddBinding(Binding* binding) {
	if (bindings.find(binding->name) != bindings.end())
		return false;

	return bindings.emplace(binding->name, binding).second;
}

bool EventManager::RemoveBinding(std::string name) {
	auto itr = bindings.find(name);
	if (itr == bindings.end()) { return false; }
	delete itr->second;
	bindings.erase(itr);
	return true;
}

void EventManager::SetCurrentState(StateType state) {
	currentState = state;
}

void EventManager::SetFocus(const bool& focus) { hasFocus = focus; }

void EventManager::HandleEvent(sf::Event& event) {
	// Handling SFML events.
	for (auto& b_itr : bindings) {
		Binding* bind = b_itr.second;
		for (auto& e_itr : bind->events) {
			EventType sfmlEvent = (EventType)event.type;
			if (e_itr.first == EventType::GUI_Click || e_itr.first == EventType::GUI_Release || e_itr.first == EventType::GUI_Hover || e_itr.first == EventType::GUI_Leave)
			{
				continue;
			}
			if (e_itr.first != sfmlEvent) { continue; }
			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
				if (e_itr.second.code == event.key.code) {
					// Matching event/keystroke.
					// Increase count.
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
					break;
				}
			}
			else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp) {
				if (e_itr.second.code == event.mouseButton.button) {
					// Matching event/keystroke.
					// Increase count.
					bind->details.mouse.x = event.mouseButton.x;
					bind->details.mouse.y = event.mouseButton.y;
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
					break;
				}
			}
			else {
				// No need for additional checking.
				if (sfmlEvent == EventType::MouseWheel) {
					bind->details.mouseWheelDelta = event.mouseWheel.delta;
				}
				else if (sfmlEvent == EventType::WindowResized) {
					bind->details.size.x = event.size.width;
					bind->details.size.y = event.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered) {
					bind->details.textEntered = event.text.unicode;
				}
				++(bind->c);
			}
		}
	}
}

void EventManager::HandleEvent(GUI_Event& event) {
	for (auto& b_itr : bindings) {
		Binding* bind = b_itr.second;
		for (auto& e_itr : bind->events)
		{
			if (e_itr.first != EventType::GUI_Click && e_itr.first != EventType::GUI_Release &&
				e_itr.first != EventType::GUI_Hover && e_itr.first != EventType::GUI_Leave)
			{
				continue;
			}
			if ((e_itr.first == EventType::GUI_Click && event.type != GUI_EventType::Click) ||
				(e_itr.first == EventType::GUI_Release && event.type != GUI_EventType::Release) ||
				(e_itr.first == EventType::GUI_Hover && event.type != GUI_EventType::Hover) ||
				(e_itr.first == EventType::GUI_Leave && event.type != GUI_EventType::Leave))
			{
				continue;
			}
			// REPLACED WITH STRCMP!
			if (strcmp(e_itr.second.gui.interface, event.interface) ||
				strcmp(e_itr.second.gui.element, event.element))
			{
				continue;
			}
			bind->details.guiInterface = event.interface;
			bind->details.guiElement = event.element;
			++(bind->c);
		}
	}
}

void EventManager::Update() {
	if (!hasFocus) { return; }
	for (auto& b_itr : bindings) {
		Binding* bind = b_itr.second;
		for (auto& e_itr : bind->events) {
			switch (e_itr.first) {
			case(EventType::Keyboard):
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.code))) {
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
				}
				break;
			case(EventType::Mouse):
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_itr.second.code))) {
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
				}
				break;
			case(EventType::Joystick):
				// Up for expansion.
				break;
			}
		}

		if (bind->events.size() == bind->c) {
			auto stateCallbacks = callbacks.find(currentState);
			auto otherCallbacks = callbacks.find(StateType(0));

			if (stateCallbacks != callbacks.end()) {
				auto callItr = stateCallbacks->second.find(bind->name);
				if (callItr != stateCallbacks->second.end()) {
					// Pass in information about events.
					callItr->second(&bind->details);
				}
			}

			if (otherCallbacks != callbacks.end()) {
				auto callItr = otherCallbacks->second.find(bind->name);
				if (callItr != otherCallbacks->second.end()) {
					// Pass in information about events.
					callItr->second(&bind->details);
				}
			}
		}
		bind->c = 0;
		bind->details.Clear();
	}
}

void EventManager::LoadBindings() {
	std::string delimiter = ":";

	std::ifstream bindings;
	bindings.open("Resources/keys.cfg");
	if (!bindings.is_open()) { std::cout << "! Failed loading keys.cfg." << std::endl; return; }
	std::string line;
	while (std::getline(bindings, line)) {
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		Binding* bind = new Binding(callbackName);
		while (!keystream.eof()) {
			std::string keyval;
			keystream >> keyval;
			if (keystream.fail()) {
				keystream.clear();
				break;
			}
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos) { delete bind; bind = nullptr; break; }
			EventType type = EventType(stoi(keyval.substr(start, end - start)));

			EventInfo eventInfo;
			if (type == EventType::GUI_Click || type == EventType::GUI_Release ||
				type == EventType::GUI_Hover || type == EventType::GUI_Leave)
			{
				start = end + delimiter.length();
				end = keyval.find(delimiter, start);
				std::string window = keyval.substr(start, end - start);
				std::string element;
				if (end != std::string::npos) {
					start = end + delimiter.length();
					end = keyval.length();
					element = keyval.substr(start, end);
				}
				char* w = new char[window.length() + 1]; // +1 for \0
				char* e = new char[element.length() + 1];

				strcpy(w, window.c_str());
				strcpy(e, element.c_str());

				eventInfo.gui.interface = w;
				eventInfo.gui.element = e;
			}
			else {
				int code = stoi(keyval.substr(end + delimiter.length(),
					keyval.find(delimiter, end + delimiter.length())));
				eventInfo.code = code;
			}
			bind->BindEvent(type, eventInfo);
		}

		if (!AddBinding(bind)) { delete bind; }
		bind = nullptr;
	}
	bindings.close();
}
