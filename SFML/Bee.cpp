#include "Bee.h"
#include "StateManager.h"

Bee::Bee(StateManager* manager) :
	GameObject(manager),
	window(nullptr),
	mt(rd()),
	randomPos(1, 10),
	randomSpeed(100, 250),
	randomDirection(0, 1),
	direction(Direction::Left),
	speed(200)
{
	window = stateMgr->GetContext()->window;
	windowSize = sf::Vector2u(1920, 1080);

	stateMgr->GetContext()->textureManager->RequireResource("bee");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("bee"));
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width * 0.5, sprite.getLocalBounds().height * 0.5));
	sprite.setScale(2, 2);

	int y = windowSize.y * 0.3;
	randomPos = std::uniform_int_distribution<int>(sprite.getGlobalBounds().height * 0.5, y);
	initRandomPos = std::uniform_int_distribution<int>(0, windowSize.x);

	InitRandomPosition();
}

Bee::~Bee() {}

void Bee::Draw() {
	window->GetRenderWindow()->draw(sprite);
}

void Bee::Update(const sf::Time& deltaTime) {
	if (direction == Direction::Left) {
		sprite.setPosition(sf::Vector2f(sprite.getPosition().x + speed * deltaTime.asSeconds(), sprite.getPosition().y));
		if (sprite.getPosition().x > windowSize.x + sprite.getGlobalBounds().width * 0.5) {
			SetRandomPosition();
		}
	}
	else {
		sprite.setPosition(sf::Vector2f(sprite.getPosition().x - speed * deltaTime.asSeconds(), sprite.getPosition().y));
		if (sprite.getPosition().x < -sprite.getGlobalBounds().width * 0.5) {
			SetRandomPosition();
		}
	}
}

void Bee::SetRandomPosition()
{
	if (randomDirection(mt) == static_cast<int>(Direction::Left)) {
		direction = Direction::Left;
		sprite.setPosition(sf::Vector2f(-(sprite.getGlobalBounds().width * 0.5), randomPos(mt)));
		speed = randomSpeed(mt);
		sprite.setScale(2, 2);
	}
	else {
		direction = Direction::Right;
		sprite.setPosition(sf::Vector2f(sprite.getGlobalBounds().width * 0.5 + windowSize.x, randomPos(mt)));
		speed = randomSpeed(mt);
		sprite.setScale(-2, 2);
	}
}

void Bee::InitRandomPosition()
{
	if (randomDirection(mt) == static_cast<int>(Direction::Left)) {
		direction = Direction::Left;
		sprite.setPosition(initRandomPos(mt), randomPos(mt));
		speed = randomSpeed(mt);
		sprite.setScale(2, 2);
	}
	else {
		direction = Direction::Right;
		sprite.setPosition(initRandomPos(mt), randomPos(mt));
		speed = randomSpeed(mt);
		sprite.setScale(-2, 2);
	}
}
