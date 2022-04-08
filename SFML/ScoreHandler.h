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
	bool isTimesUp();

private:
	StateManager* stateMgr;

	sf::RectangleShape scoreRect;
	unsigned int score;
	sf::Text scoreText; //log chopped

	sf::Vector2f timeRectSize;
	sf::RectangleShape timeRect;
	float timeShrink;

	bool timesUp;
};

