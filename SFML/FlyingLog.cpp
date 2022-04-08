#include "FlyingLog.h"
#include "StateManager.h"

FlyingLog::FlyingLog(StateManager* manager, const sf::Vector2f& pos, const LogDirection& dir) :
	GameObject(manager),
	direction(dir),
	logSpeed(3000, 700),
	rotation(100),
	canBeDeleted(false)
{
	stateMgr->GetContext()->textureManager->RequireResource("log");
	log.setTexture(*stateMgr->GetContext()->textureManager->GetResource("log"));
	log.setOrigin(log.getLocalBounds().width * 0.5, log.getLocalBounds().height);
	log.setPosition(pos);
}

FlyingLog::~FlyingLog(){}

void FlyingLog::Draw()
{
	sf::RenderWindow* window = stateMgr->GetContext()->window->GetRenderWindow();
	window->draw(log);
}

void FlyingLog::Update(const sf::Time& time)
{
	if (direction == LogDirection::Right) {
		log.setPosition(log.getPosition().x - logSpeed.x * time.asSeconds(), log.getPosition().y - logSpeed.y * time.asSeconds());
		log.rotate(rotation * time.asSeconds());
	}
	else {
		log.setPosition(log.getPosition().x + logSpeed.x * time.asSeconds(), log.getPosition().y - logSpeed.y * time.asSeconds());
		log.setRotation(log.getRotation() - rotation * time.asSeconds());
	}


	if (log.getPosition().x < -200 || log.getPosition().x > 2100) {
		canBeDeleted = true;
	}
}

void FlyingLog::SetPosition(const sf::Vector2f& pos)
{
	log.setPosition(pos);
}

void FlyingLog::Launch(const LogDirection& dir)
{
	direction = dir;
}

bool FlyingLog::CanBeDeleted()
{
	return canBeDeleted;
}
