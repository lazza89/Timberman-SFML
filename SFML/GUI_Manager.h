#pragma once
#include <SFML/Graphics.hpp>
#include "EventManager.h"

#include "GUI_Interface.h"
#include "GUI_Event.h"
#include "GUI_Label.h"
#include "GUI_Scrollbar.h"
#include "GUI_Textfield.h"

#include <unordered_map>
#include <functional>
#include <fstream>

using GUI_Interfaces = std::map<std::string, GUI_Interface*>;
using GUI_Container = std::unordered_map<StateType, GUI_Interfaces>;
using GUI_Events = std::unordered_map<StateType, std::vector<GUI_Event>>;
using GUI_Factory = std::unordered_map<GUI_ElementType, std::function<GUI_Element*(GUI_Interface*)>>;
using GUI_ElemTypes = std::unordered_map <std::string,GUI_ElementType>;

struct SharedContext;
class GUI_Manager{
	friend class GUI_Interface;
public:
	GUI_Manager(EventManager* evMgr, SharedContext* shared);
	~GUI_Manager();

	bool AddInterface(const StateType& state, const std::string& name);
	GUI_Interface* GetInterface(const StateType& state,const std::string& name);
	bool RemoveInterface(const StateType& state, const std::string& name);

	bool LoadInterface(const StateType& state, const std::string& interface, const std::string& name);

	void SetCurrentState(const StateType& state);

	SharedContext* GetContext();

	void DefocusAllInterfaces();

	void HandleClick(EventDetails* details);
	void HandleRelease(EventDetails* details);
	void HandleTextEntered(EventDetails* details);

	void AddEvent(GUI_Event event);
	bool PollEvent(GUI_Event& event);

	void Update(float dT);
	void Render(sf::RenderWindow* wind);

	template<class T>
	void RegisterElement(const GUI_ElementType& id){
		factory[id] = [](GUI_Interface* owner) -> GUI_Element*{ 
			return new T("",owner);
		};
	}
private:
	GUI_Element* CreateElement(const GUI_ElementType& id, GUI_Interface* owner);
	GUI_ElementType StringToType(const std::string& string);
	bool LoadStyle(const std::string& file, GUI_Element* element);

	GUI_Container interfaces;
	GUI_Events events;
	SharedContext* context;
	EventManager* eventMgr;
	StateType currentState;
	GUI_Factory factory;
	GUI_ElemTypes elemTypes;
};