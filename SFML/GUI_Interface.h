#pragma once
#include <SFML/Graphics.hpp>

enum class GUI_ElementState {
	Neutral,
	Focused,
	Clicked
};

struct GUI_Style {
	GUI_Style() : textSize(12), textCenterOrigin(false),
		backgroundImageColor(255, 255, 255, 255)
	{
		sf::Color none = sf::Color(0, 0, 0, 0);
		backgroundColor = none;
		elementColor = none;
		textColor = none;
	}
	//Background propieties
	sf::Color backgroundColor;
	sf::Color elementColor;
	std::string backgroundImage;
	sf::Color backgroundImageColor;
	//text prop
	sf::Color textColor;
	std::string textFont;
	sf::Vector2f textPadding;
	unsigned int textSize;
	bool textCenterOrigin;
	//Glyph propieties
	std::string glyph;
	sf::Vector2f glyphPadding;
};

struct GUI_Visual {
	sf::RectangleShape backgroundSolid;
	sf::Sprite backgroundImage;
	sf::Sprite glyph;
	sf::Text text;
};