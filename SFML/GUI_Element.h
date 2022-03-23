#pragma once
#include <unordered_map>

enum class GUI_ElementType {
	Window,
	Label,
	Button,
	Scrollbar,
	Textfield
};

using ElementStyles = std::unordered_map<GUI_ElementState, GUI_Style>;

class GUI_Interface;

class GUI_Element
{
	friend class GUI_Interface;

public:
	GUI_Element(const std::string& name, const GUI_ElementType& type, GUI_Interface* owner);
	virtual  ~GUI_Element();

	virtual void ReadIn(std::stringstream& stream) = 0;
	virtual void OnClick(const sf::Vector2f& mousePos) = 0;
	virtual void OnRelease() = 0;
	virtual void OnHover(const sf::Vector2f& mousePos) = 0;
	virtual void OnLeave() = 0;
	virtual void Update(float dT) = 0;
	virtual void Draw(sf::RenderTarget* target) = 0;

	virtual void UpdateStyle(const GUI_ElementState& state, const GUI_Style& style);
	virtual void ApplyStyle();

	friend std::stringstream& operator >>(std::stringstream& stream, GUI_Element& b) {
		b.ReadIn(stream);
		return stream;
	}

protected:
	void ApplyTextStyle();
	void ApplyBgStyle();
	void ApplyGlyphStyle();

	void RequireTexture(const std::string& name);
	void RequireFont(const std::string& name);
	void RealeaseTexture(const std::string& name);
	void ReleaseFont(const std::string& name);
	void ReleaseResources();
	std::string name;
	sf::Vector2f position;
	ElementStyles style;
	GUI_Visual visual;
	GUI_ElementType type;
	GUI_ElementState state;
	GUI_Interface* owner;

	bool needsRedraw;
	bool active;
	bool isControl;


};

