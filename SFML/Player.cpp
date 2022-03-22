#include "Player.h"
#include "StateManager.h"

Player::Player(StateManager* manager) :
	GameObject(manager),
	direction(PlayerDirection::Left)
{
	window = stateMgr->GetContext()->window->GetRenderWindow();
	stateMgr->GetContext()->textureManager->RequireResource("player");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("player"));
	sprite.setOrigin(sprite.getLocalBounds().width * 0.5, sprite.getLocalBounds().height * 0.5);
	sprite.setScale(-1, 1);
}

Player::~Player()
{
}

void Player::Draw()
{
	window->draw(sprite);
}

void Player::Update(const sf::Time& deltaTime)
{
}

void Player::SetPosition(const sf::Vector2f& pos, PlayerDirection direction)
{
	this->direction = direction;
	if (direction == PlayerDirection::Left) {
		sprite.setScale(-1, 1);
	}
	else {
		sprite.setScale(1, 1);
	}
	sprite.setPosition(pos);
}

PlayerDirection Player::GetDirection()
{
	return direction;
}
