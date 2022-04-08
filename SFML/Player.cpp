#include "Player.h"
#include "StateManager.h"

Player::Player(StateManager* manager) :
	GameObject(manager),
	direction(PlayerDirection::Left)
{
	window = stateMgr->GetContext()->window;
	stateMgr->GetContext()->textureManager->RequireResource("player");
	stateMgr->GetContext()->textureManager->RequireResource("rip");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("player"));
	sprite.setOrigin(sprite.getLocalBounds().width * 0.5, sprite.getLocalBounds().height * 0.5);
	sprite.setScale(-1, 1);

	ripSprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("rip"));
	ripSprite.setOrigin(ripSprite.getLocalBounds().width * 0.5, ripSprite.getLocalBounds().height * 0.5);
	ripSprite.setScale(1.5, 1.5);
}

Player::~Player()
{
}

void Player::Draw()
{
	window->GetRenderWindow()->draw(sprite);
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

void Player::Dead()
{
	ripSprite.setPosition(sprite.getPosition());
	sprite = ripSprite;
}
