#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <string>
#include <unordered_map>

#include "GUI_Style.h"
#include <sstream>

enum class GUI_ElementType{ 
	None = -1,
	Window,
	Label,
	Button,
	Scrollbar,
	Textfield
};

using ElementStyles = std::unordered_map<GUI_ElementState, GUI_Style>;

class GUI_Interface;

class GUI_Element{
	friend class GUI_Interface;
public:
	GUI_Element(const std::string& name, const GUI_ElementType& type, GUI_Interface* owner);
	virtual ~GUI_Element();

	// Event methods.
	virtual void ReadIn(std::stringstream& stream) = 0;
	virtual void OnClick(const sf::Vector2f& mousePos) = 0;
	virtual void OnRelease() = 0;
	virtual void OnHover(const sf::Vector2f& mousePos) = 0;
	virtual void OnLeave() = 0;
	virtual void Update(float dT) = 0;
	virtual void Draw(sf::RenderTarget* target) = 0;

	// Non pure-virtual methods.
	virtual void UpdateStyle(const GUI_ElementState& state, const GUI_Style& style);
	virtual void ApplyStyle();

	GUI_ElementType GetType();

	const std::string& GetName() const;
	void SetName(const std::string& name);
	const sf::Vector2f& GetPosition() const;
	void SetPosition(const sf::Vector2f& pos);
	const sf::Vector2f& GetSize() const;
	GUI_ElementState GetState() const;
	void SetState(const GUI_ElementState& state);
	void SetRedraw(const bool& redraw);
	bool NeedsRedraw() const;
	void SetOwner(GUI_Interface* owner);
	GUI_Interface* GetOwner() const;
	bool HasOwner() const;
	bool IsActive() const;
	void SetActive(const bool& active);
	bool IsInside(const sf::Vector2f& point) const;
	sf::Vector2f GetGlobalPosition() const;
	bool IsControl() const;
	std::string GetText() const;
	void SetText(const std::string& text);

	friend std::stringstream& operator >>(
		std::stringstream& stream, GUI_Element& b)
	{
		b.ReadIn(stream);
		return stream;
	}
protected:
	void ApplyTextStyle();
	void ApplyBgStyle();
	void ApplyGlyphStyle();

	void RequireTexture(const std::string& name);
	void RequireFont(const std::string& name);
	void ReleaseTexture(const std::string& name);
	void ReleaseFont(const std::string& name);
	void ReleaseResources();
	std::string name;
	sf::Vector2f position;
	ElementStyles style; // Style of drawables.
	GUI_Visual visual; // Drawable bits.
	GUI_ElementType type;
	GUI_ElementState state;
	GUI_Interface* owner;

	bool needsRedraw;
	bool active;
	bool isControl;
};