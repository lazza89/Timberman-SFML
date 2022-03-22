#pragma once
#include "GameObject.h"

enum class PlayerDirection {
	Left,
	Right
};

class Player : public GameObject
{
public:
	Player(StateManager* manager);
	~Player();

	void Draw() override;
	void Update(const sf::Time& deltaTime) override;

	void SetPosition(const sf::Vector2f& pos, PlayerDirection direction);

private:
	sf::RenderWindow* window;
	sf::Sprite sprite;

	PlayerDirection direction;

};

