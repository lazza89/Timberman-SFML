#pragma once
#include <SFML/Graphics.hpp>

class StateManager;

class ScoreHandler
{
public:
	ScoreHandler(StateManager* manager);
	~ScoreHandler();

	void Draw();
	void Update(const sf::Time& time);

	void AddScore(int n);
	void StartTime();



private:
	StateManager* stateMgr;

	sf::Clock clock;
	sf::Time time;

	sf::RectangleShape scoreRect;
	unsigned int score;
	sf::Text scoreText; //log chopped

};

