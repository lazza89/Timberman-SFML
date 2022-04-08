#pragma once
#include "GameObject.h"
#include "Window.h"

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
	PlayerDirection GetDirection();
	void Dead();

private:
	Window* window;
	sf::Sprite sprite;
	sf::Sprite ripSprite;

	PlayerDirection direction;

};

