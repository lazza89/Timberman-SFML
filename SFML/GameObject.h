#pragma once
#include <SFML/Graphics.hpp>

class StateManager;

class GameObject {
public:
	GameObject(StateManager* manager = nullptr) : stateMgr(manager){}
	virtual ~GameObject() {}

	virtual void Draw() = 0;
	virtual void Update(const sf::Time& deltaTime) = 0;

protected:
	StateManager* stateMgr;
};