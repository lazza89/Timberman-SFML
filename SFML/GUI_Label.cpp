#include "GUI_Label.h"
#include <algorithm>
#include <iostream>
#include "Utilities.h"

GUI_Label::GUI_Label(const std::string& name, GUI_Interface* owner) : 
	GUI_Element(name, GUI_ElementType::Label, owner)
{}
GUI_Label::~GUI_Label(){}

void GUI_Label::ReadIn(std::stringstream& stream){
	std::string content;
	Utils::ReadQuotedString(stream, content);
	visual.text.setString(content);
}

void GUI_Label::OnClick(const sf::Vector2f& mousePos){
	SetState(GUI_ElementState::Clicked);
}
void GUI_Label::OnRelease(){
	SetState(GUI_ElementState::Neutral);
}
void GUI_Label::OnHover(const sf::Vector2f& mousePos){
	SetState(GUI_ElementState::Focused);
}
void GUI_Label::OnLeave(){
	SetState(GUI_ElementState::Neutral);
}
void GUI_Label::Update(float dT){}

void GUI_Label::Draw(sf::RenderTarget* target){
	target->draw(visual.backgroundSolid);
	if (style[state].glyph != ""){ target->draw(visual.glyph); }
	target->draw(visual.text);
}