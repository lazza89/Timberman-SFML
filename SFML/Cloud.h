#pragma once
#include "GameObject.h"
#include <random>

class Cloud : public GameObject
{
public:
	Cloud(StateManager* manager = nullptr);
	~Cloud();

	void Draw() override;
	void Update(const sf::Time& deltaTime) override;


private:
	sf::Sprite sprite;
	sf::RenderWindow* window;
	sf::Vector2u windowSize;

	float speed;

	std::random_device rd;
	std::mt19937 mt;
	std::uniform_int_distribution<int> randomPos;
	std::uniform_int_distribution<int> randomSpeed;
	std::uniform_int_distribution<int> initRandomPos;

	void SetRandomPosition();
	void InitRandomPosition();
};

