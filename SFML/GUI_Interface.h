#pragma once
#include "GUI_Element.h"
#include <unordered_map>

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

using Elements = std::unordered_map<std::string,GUI_Element*>;

class GUI_Manager;

class GUI_Interface : public GUI_Element{
	friend class GUI_Element;
	friend class GUI_Manager;
public:
	GUI_Interface(const std::string& name, GUI_Manager* guiManager);
	~GUI_Interface();

	void SetPosition(const sf::Vector2f& pos);

	bool AddElement(const GUI_ElementType& type, const std::string& name);
	GUI_Element* GetElement(const std::string& name) const;
	bool RemoveElement(const std::string& name);

	bool HasParent() const;
	GUI_Manager* GetManager() const;

	bool IsInside(const sf::Vector2f& point) const;

	void Focus();
	void Defocus();
	bool IsFocused();

	void ReadIn(std::stringstream& stream);
	void OnClick(const sf::Vector2f& mousePos);
	void OnRelease();
	void OnHover(const sf::Vector2f& mousePos);
	void OnLeave();
	void OnTextEntered(const char& chr);

	bool IsBeingMoved() const;
	bool IsMovable() const;
	void BeginMoving();
	void StopMoving();

	void ApplyStyle();

	const sf::Vector2f& GetPadding() const;
	void SetPadding(const sf::Vector2f& padding);
	sf::Vector2f GetGlobalPosition() const;
	const sf::Vector2f& GetContentSize() const;

	void Redraw();
	bool NeedsContentRedraw() const;
	void RedrawContent();
	bool NeedsControlRedraw() const;
	void RedrawControls();

	void ToggleTitleBar();

	void Update(float dT);
	void Draw(sf::RenderTarget* target);

	void UpdateScrollHorizontal(unsigned int percent);
	void UpdateScrollVertical(unsigned int percent);
private:
	void DefocusTextfields();
	Elements elements;
	sf::Vector2f elementPadding;

	GUI_Interface* parent;
	GUI_Manager* guiManager;

	sf::RenderTexture* backdropTexture;
	sf::Sprite backdrop;

	// Movement.
	sf::RectangleShape titleBar;
	sf::Vector2f moveMouseLast;
	bool showTitleBar;
	bool movable;
	bool beingMoved;
	bool focused;

	// Variable size.
	void AdjustContentSize(const GUI_Element* reference = nullptr);
	void SetContentSize(const sf::Vector2f& vec);
	sf::RenderTexture* contentTexture;
	sf::Sprite content;
	sf::Vector2f contentSize;
	int scrollHorizontal;
	int scrollVertical;
	bool contentRedraw;

	// Control layer.
	sf::RenderTexture* controlTexture;
	sf::Sprite control;
	bool controlRedraw;
};