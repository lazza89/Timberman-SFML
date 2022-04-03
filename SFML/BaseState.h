#pragma once
#include <SFML/Graphics.hpp>
class StateManager;

class BaseState{
friend class StateManager;
public:
	BaseState(StateManager* stateManager) 
		:stateMgr(stateManager), transparent(false),
		transcendent(false)
	{}
	virtual ~BaseState(){}

	virtual void OnCreate() = 0;
	virtual void OnDestroy() = 0;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;

	virtual void Update(const sf::Time& time) = 0;
	virtual void Draw() = 0;

	void SetTransparent(const bool& tmpTransparent){ transparent = tmpTransparent; }
	bool IsTransparent()const{ return transparent; }
	void SetTranscendent(const bool& tmpTranscendence){ transcendent = tmpTranscendence; }
	bool IsTranscendent()const{ return transcendent; }
	sf::View& GetView(){ return view; }
	StateManager* GetStateManager(){ return stateMgr; }
protected:
	StateManager* stateMgr;
	bool transparent;
	bool transcendent;
	sf::View view;
};