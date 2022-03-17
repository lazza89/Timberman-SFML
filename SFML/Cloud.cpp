#pragma once
#include "Cloud.h"
#include "StateManager.h"

Cloud::Cloud(StateManager* manager) :
	GameObject(manager),
	window(nullptr),
	mt(rd()),
	randomPos(1, 10),
	randomSpeed(20, 120),
	speed(10)
{
	window = stateMgr->GetContext()->window->GetRenderWindow();
	windowSize = window->getSize();

	stateMgr->GetContext()->textureManager->RequireResource("cloud");
	sprite.setTexture(*stateMgr->GetContext()->textureManager->GetResource("cloud"));
	sprite.setOrigin(sf::Vector2f(sprite.getLocalBounds().width * 0.5, sprite.getLocalBounds().height * 0.5));
	sprite.setScale(2, 2);

	int y = windowSize.y * 0.2;
	randomPos = std::uniform_int_distribution<int>(sprite.getGlobalBounds().height * 0.5, y);
	initRandomPos = std::uniform_int_distribution<int>(0, windowSize.x);

	InitRandomPosition();
}

Cloud::~Cloud() {}

void Cloud::Draw() {
	window->draw(sprite);
}

void Cloud::Update(const sf::Time& deltaTime) {
	sprite.setPosition(sf::Vector2f(sprite.getPosition().x - speed * deltaTime.asSeconds(), sprite.getPosition().y));
	if (sprite.getPosition().x < -sprite.getGlobalBounds().width *0.5) {
		SetRandomPosition();
	}
}

void Cloud::SetRandomPosition()
{
	sprite.setPosition(sf::Vector2f(sprite.getGlobalBounds().width * 0.5 + windowSize.x, randomPos(mt)));
	speed = randomSpeed(mt);
	sprite.setScale(-2, 2);
}

void Cloud::InitRandomPosition()
{
	sprite.setPosition(initRandomPos(mt), randomPos(mt));
	speed = randomSpeed(mt);
	sprite.setScale(-2, 2);
}


