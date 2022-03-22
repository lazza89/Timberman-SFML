#include "Player.h"
#include "StateManager.h"

Player::Player(StateManager* manager) :
	GameObject(manager),
	direction(PlayerDirection::Left)
{
	window = stateMgr->GetContext()->window->GetRenderWindow();
	stateMgr->GetContext()->textureManager->RequireResource("player");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("player"));

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
	sprite.setPosition(pos);
}
