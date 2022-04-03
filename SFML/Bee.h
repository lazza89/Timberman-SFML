#pragma once
#include "GameObject.h"
#include <random>
#include "Window.h"

enum class Direction {
	Left,
	Right
};

class Bee : public GameObject
{
public:
	Bee(StateManager* manager = nullptr);
	~Bee();

	void Draw() override;
	void Update(const sf::Time& deltaTime) override;


private:
	sf::Sprite sprite;
	Window* window;
	sf::Vector2u windowSize;

	float speed;
	Direction direction;

	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> randomPos;
	std::uniform_int_distribution<int> randomSpeed;
	std::uniform_int_distribution<int> randomDirection;
	std::uniform_int_distribution<int> initRandomPos;

	void SetRandomPosition();
	void InitRandomPosition();
};

