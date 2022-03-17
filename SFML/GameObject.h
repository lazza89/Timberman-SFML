#pragma once
#include <SFML/System/Time.hpp>
#include "StateManager.h"

class GameObject {
public:
	GameObject(StateManager* manager) : stateMgr(manager){}
	virtual ~GameObject() = 0;

	virtual void Render() = 0;
	virtual void Update(const sf::Time& deltaTime) = 0;

protected:
	StateManager* stateMgr;

};