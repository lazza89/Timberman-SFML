#pragma once
#include "GameObject.h"

enum class LogDirection {
	Left,
	Right
};

class FlyingLog : public GameObject
{
public:
	FlyingLog(StateManager* manager, const sf::Vector2f& pos, const LogDirection& dir);
	~FlyingLog();

	void Draw();
	void Update(const sf::Time& time);
	bool canBeDeleted;

private:
	sf::Sprite log;
	LogDirection direction;
	sf::Vector2f logSpeed;
	float rotation;

};

