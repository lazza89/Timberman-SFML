#include "GUI_Scrollbar.h"
#include "GUI_Interface.h"
#include "GUI_Manager.h"
#include "SharedContext.h"

GUI_Scrollbar::GUI_Scrollbar(const std::string& name, GUI_Interface* owner) : 
	GUI_Element(name, GUI_ElementType::Scrollbar, owner),
	percentage(0),
	sliderType(SliderType::Vertical)
{
	isControl = true;
}

GUI_Scrollbar::~GUI_Scrollbar(){}

void GUI_Scrollbar::SetPosition(const sf::Vector2f& pos){
	GUI_Element::SetPosition(pos);
	if (sliderType == SliderType::Horizontal){ position.x = 0; }
	else { position.y = 0; }
}
void GUI_Scrollbar::ReadIn(std::stringstream& stream){
	std::string type;
	stream >> type;
	if (type == "Horizontal"){ sliderType = SliderType::Horizontal; }
	else { sliderType = SliderType::Vertical; }

	if (sliderType == SliderType::Horizontal){ slider.setPosition(0, GetPosition().y); }
	else { slider.setPosition(GetPosition().x, 0); }
}
void GUI_Scrollbar::OnClick(const sf::Vector2f& mousePos){
	if (!slider.getGlobalBounds().contains(mousePos - owner->GetPosition())){ return; }
	SetState(GUI_ElementState::Clicked);
	moveMouseLast = mousePos;
}
void GUI_Scrollbar::OnRelease(){
	SetState(GUI_ElementState::Neutral);
}
void GUI_Scrollbar::OnHover(const sf::Vector2f& mousePos){
	SetState(GUI_ElementState::Focused);
}
void GUI_Scrollbar::OnLeave(){
	SetState(GUI_ElementState::Neutral);
}
void GUI_Scrollbar::UpdateStyle(const GUI_ElementState& l_state, const GUI_Style& l_style)
{
	GUI_Element::UpdateStyle(state, l_style);
	if (sliderType == SliderType::Horizontal){ 
		style[l_state].size.x = owner->GetSize().x;
	}
	else { 
		style[l_state].size.y = owner->GetSize().y;
	}
}

void GUI_Scrollbar::ApplyStyle(){
	GUI_Element::ApplyStyle();
	slider.setFillColor(style[state].elementColor);
	bool horizontal = sliderType == SliderType::Horizontal;
	auto& bgSolid = visual.backgroundSolid;

	SetPosition((horizontal ? sf::Vector2f(0, owner->GetSize().y - bgSolid.getSize().y) : sf::Vector2f(owner->GetSize().x - bgSolid.getSize().x, 0)));

	bgSolid.setSize((horizontal ? sf::Vector2f(owner->GetSize().x, style[state].size.y) : sf::Vector2f(style[state].size.x, owner->GetSize().y)));

	slider.setPosition( (horizontal ? slider.getPosition().x : GetPosition().x), (horizontal ? GetPosition().y : slider.getPosition().y));

	float SizeFactor = (horizontal ? owner->GetContentSize().x / owner->GetSize().x : owner->GetContentSize().y / owner->GetSize().y);

	if (SizeFactor < 1.f){ 
		SizeFactor = 1.f;
	}

	float SliderSize = (horizontal ? owner->GetSize().x : owner->GetSize().y) / SizeFactor;

	slider.setSize((horizontal ? sf::Vector2f(SliderSize, bgSolid.getSize().y) : sf::Vector2f(bgSolid.getSize().x, SliderSize)));
	bgSolid.setPosition(GetPosition());
}

void GUI_Scrollbar::Update(float dT){
	// Mouse-drag code.
	if (GetState() != GUI_ElementState::Clicked){ return; }
	SharedContext* context = owner->GetManager()->GetContext();
	sf::Vector2f mousePos = sf::Vector2f(context->eventManager->GetMousePos(context->window->GetRenderWindow()));
	if (moveMouseLast == mousePos){ return; }
	sf::Vector2f difference = mousePos - moveMouseLast;
	moveMouseLast = mousePos;

	bool horizontal = sliderType == SliderType::Horizontal;
	slider.move((horizontal ? difference.x : 0), (horizontal ? 0 : difference.y));
	if (horizontal && slider.getPosition().x < 0){
		slider.setPosition(0, slider.getPosition().y);
	} else if (slider.getPosition().y < 0){
		slider.setPosition(slider.getPosition().x, 0);
	}
	if (horizontal && (slider.getPosition().x + slider.getSize().x > owner->GetSize().x)){
		slider.setPosition(owner->GetSize().x - slider.getSize().x, slider.getPosition().y);
	} else if (slider.getPosition().y + slider.getSize().y > owner->GetSize().y){
		slider.setPosition(slider.getPosition().x, owner->GetSize().y - slider.getSize().y);
	}
	float WorkArea = (horizontal ? owner->GetSize().x - slider.getSize().x
		: owner->GetSize().y - slider.getSize().y);
	int percentage = ((horizontal ?
		slider.getPosition().x : slider.getPosition().y) / WorkArea) * 100;
	if (horizontal){ owner->UpdateScrollHorizontal(percentage); }
	else { owner->UpdateScrollVertical(percentage); }
	SetRedraw(true);
}
void GUI_Scrollbar::Draw(sf::RenderTarget* target){
	target->draw(visual.backgroundSolid);
	target->draw(slider);
}