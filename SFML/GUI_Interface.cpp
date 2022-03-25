#include "GUI_Interface.h"
#include "GUI_Manager.h"
#include "SharedContext.h"
#include "Utilities.h"

GUI_Interface::GUI_Interface(const std::string& name, GUI_Manager* guiManager) : 
	GUI_Element(name, GUI_ElementType::Window, this), 
	parent(nullptr), 
	guiManager(guiManager),
	movable(false), 
	beingMoved(false), 
	showTitleBar(false), 
	focused(false), 
	scrollHorizontal(0),
	scrollVertical(0),
	contentRedraw(true), 
	controlRedraw(true)
{
	backdropTexture = new sf::RenderTexture();
	contentTexture = new sf::RenderTexture();
	controlTexture = new sf::RenderTexture();
}

GUI_Interface::~GUI_Interface(){
	delete backdropTexture;
	delete contentTexture;
	delete controlTexture;
	for (auto &itr : elements){
		delete itr.second;
	}
}

void GUI_Interface::SetPosition(const sf::Vector2f& pos){
	GUI_Element::SetPosition(pos);
	backdrop.setPosition(pos);
	content.setPosition(pos);
	control.setPosition(pos);
	titleBar.setPosition(position.x, position.y - titleBar.getSize().y);
	visual.text.setPosition(titleBar.getPosition() + style[state].textPadding);
}

bool GUI_Interface::AddElement(const GUI_ElementType& type, 
	const std::string& name)
{
	if (elements.find(name) != elements.end()){ 
		return false;
	}
	GUI_Element* element = nullptr;
	element = guiManager->CreateElement(type, this);
	if (!element){ 
		return false;
	}
	element->SetName(name);
	element->SetOwner(this);
	elements.emplace(name, element);
	contentRedraw = true;
	controlRedraw = true;
	return true;
}

GUI_Element* GUI_Interface::GetElement(const std::string& name) const{
	auto itr = elements.find(name);
	return(itr != elements.end() ? itr->second : nullptr);
}

bool GUI_Interface::RemoveElement(const std::string& name){
	auto itr = elements.find(name);
	if (itr == elements.end()){ return false; }
	delete itr->second;
	elements.erase(itr);
	contentRedraw = true;
	controlRedraw = true;
	AdjustContentSize();
	return true;
}

bool GUI_Interface::HasParent() const{
	return parent != nullptr; 
}

GUI_Manager* GUI_Interface::GetManager() const{ 
	return guiManager;
}

bool GUI_Interface::IsInside(const sf::Vector2f& point) const{
	if (GUI_Element::IsInside(point)){
		return true; 
	}
	return titleBar.getGlobalBounds().contains(point);
}

void GUI_Interface::Focus(){ focused = true; }
void GUI_Interface::Defocus(){ focused = false; }
bool GUI_Interface::IsFocused(){ return focused; }

void GUI_Interface::ReadIn(std::stringstream& stream){
	std::string movableState;
	std::string titleShow;
	std::string title;
	stream >> elementPadding.x >> elementPadding.y >> movableState >> titleShow; 
	Utils::ReadQuotedString(stream, title);
	visual.text.setString(title);
	if (movableState == "Movable"){ movable = true; }
	if (titleShow == "Title"){ showTitleBar = true; }
}

void GUI_Interface::OnClick(const sf::Vector2f& mousePos){
	DefocusTextfields();
	if (titleBar.getGlobalBounds().contains(mousePos) &&
		movable && showTitleBar)
	{
		beingMoved = true;
	} else {
		GUI_Event event;
		event.type = GUI_EventType::Click;
		event.interface = name.c_str();
		event.element = "";
		event.clickCoords.x = mousePos.x;
		event.clickCoords.y = mousePos.y;
		guiManager->AddEvent(event);
		for (auto &itr : elements){
			if (!itr.second->IsInside(mousePos)){ continue; }
			itr.second->OnClick(mousePos);
			event.element = itr.second->name.c_str();
			guiManager->AddEvent(event);
		}
		SetState(GUI_ElementState::Clicked);
	}
}

void GUI_Interface::OnRelease(){
	GUI_Event event;
	event.type = GUI_EventType::Release;
	event.interface = name.c_str();
	event.element = "";
	guiManager->AddEvent(event);
	for (auto &itr : elements){
		if (itr.second->GetState() != GUI_ElementState::Clicked)
		{
			continue;
		}
		itr.second->OnRelease();
		event.element = itr.second->name.c_str();
		guiManager->AddEvent(event);
	}
	SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::OnHover(const sf::Vector2f& mousePos){
	GUI_Event event;
	event.type = GUI_EventType::Hover;
	event.interface = name.c_str();
	event.element = "";
	event.clickCoords.x = mousePos.x;
	event.clickCoords.y = mousePos.y;
	guiManager->AddEvent(event);

	SetState(GUI_ElementState::Focused);
}

void GUI_Interface::OnLeave(){
	GUI_Event event;
	event.type = GUI_EventType::Leave;
	event.interface = name.c_str();
	event.element = "";
	guiManager->AddEvent(event);

	SetState(GUI_ElementState::Neutral);
}

void GUI_Interface::OnTextEntered(const char& chr){
	for (auto &itr : elements){
		if (itr.second->GetType() != GUI_ElementType::Textfield){ continue; }
		if (itr.second->GetState() != GUI_ElementState::Clicked){ continue; }
		if (chr == 8){
			// Backspace.
			const auto& text = itr.second->GetText();
			itr.second->SetText(text.substr(0, text.length() -1));
			return;
		}
		if (chr < 32 || chr > 126){ return; }
		std::string text = itr.second->GetText();
		text.push_back(chr);
		itr.second->SetText(text);
		return;
	}
}

const sf::Vector2f& GUI_Interface::GetPadding() const{ return elementPadding; }
void GUI_Interface::SetPadding(const sf::Vector2f& padding){ elementPadding = padding;  }

void GUI_Interface::Update(float dT){
	sf::Vector2f mousePos = sf::Vector2f(
		guiManager->GetContext()->eventManager->GetMousePos(
		guiManager->GetContext()->window->GetRenderWindow()));

	if (beingMoved && moveMouseLast != mousePos){
		sf::Vector2f difference = mousePos - moveMouseLast;
		moveMouseLast = mousePos;
		sf::Vector2f newPosition = position + difference;
		SetPosition(newPosition);
	}

	for (auto &itr : elements){
		if (itr.second->NeedsRedraw()){
			if (itr.second->IsControl()){ controlRedraw = true; } 
			else { contentRedraw = true; }
		}
		if (!itr.second->IsActive()){ continue; }
		itr.second->Update(dT);
		if (beingMoved){ continue; }
		GUI_Event event;
		event.interface = name.c_str();
		event.element = itr.second->name.c_str();
		event.clickCoords.x = mousePos.x;
		event.clickCoords.y = mousePos.y;
		if (IsInside(mousePos) && itr.second->IsInside(mousePos) 
			&& !titleBar.getGlobalBounds().contains(mousePos))
		{
			if (itr.second->GetState() != GUI_ElementState::Neutral){ continue; }
			itr.second->OnHover(mousePos);
			event.type = GUI_EventType::Hover;
			guiManager->AddEvent(event);
		} else if (itr.second->GetState() == GUI_ElementState::Focused){
			itr.second->OnLeave();
			event.type = GUI_EventType::Leave;
			guiManager->AddEvent(event);
		}
	}
}

void GUI_Interface::Draw(sf::RenderTarget* target){
	target->draw(backdrop);
	target->draw(content);
	target->draw(control);

	if (!showTitleBar){ return; }
	target->draw(titleBar);
	target->draw(visual.text);
}

bool GUI_Interface::IsBeingMoved() const{ return beingMoved; }
bool GUI_Interface::IsMovable() const{ return movable;  }

void GUI_Interface::BeginMoving(){
	if (!showTitleBar || !movable){ return; }
	beingMoved = true;
	SharedContext* context = guiManager->GetContext();
	moveMouseLast = sf::Vector2f(context->eventManager->GetMousePos(context->window->GetRenderWindow()));
}

void GUI_Interface::StopMoving(){ beingMoved = false; }

sf::Vector2f GUI_Interface::GetGlobalPosition() const{
	sf::Vector2f pos = position;
	GUI_Interface* i = parent;
	while (i){
		pos += i->GetPosition();
		i = i->parent;
	}
	return pos;
}

void GUI_Interface::ApplyStyle(){
	GUI_Element::ApplyStyle(); // Call base method.
	visual.backgroundSolid.setPosition(0.f,0.f);
	visual.backgroundImage.setPosition(0.f,0.f);
	titleBar.setSize(sf::Vector2f(style[state].size.x, 16.f));
	titleBar.setPosition(position.x,position.y - titleBar.getSize().y);
	titleBar.setFillColor(style[state].elementColor);
	visual.text.setPosition(titleBar.getPosition() + style[state].textPadding);
	visual.glyph.setPosition(titleBar.getPosition() + style[state].glyphPadding);
}

void GUI_Interface::Redraw(){
	if (backdropTexture->getSize().x != style[state].size.x || backdropTexture->getSize().y != style[state].size.y)
	{
		backdropTexture->create(style[state].size.x, style[state].size.y);
	}
	backdropTexture->clear(sf::Color(0, 0, 0, 0));
	ApplyStyle();
	backdropTexture->draw(visual.backgroundSolid);

	if (style[state].backgroundImage != ""){
		backdropTexture->draw(visual.backgroundImage);
	}

	backdropTexture->display();
	backdrop.setTexture(backdropTexture->getTexture());
	backdrop.setTextureRect(sf::IntRect(0, 0, style[state].size.x, style[state].size.y));
	SetRedraw(false);
}
bool GUI_Interface::NeedsContentRedraw() const{ return contentRedraw; }
void GUI_Interface::RedrawContent(){
	if (contentTexture->getSize().x != contentSize.x || contentTexture->getSize().y != contentSize.y)
	{
		contentTexture->create(contentSize.x, contentSize.y);
	}

	contentTexture->clear(sf::Color(0, 0, 0, 0));

	for (auto &itr : elements){
		GUI_Element* element = itr.second;
		if (!element->IsActive() || element->IsControl()){ continue; }
		element->ApplyStyle();
		element->Draw(contentTexture);
		element->SetRedraw(false);
	}

	contentTexture->display();
	content.setTexture(contentTexture->getTexture());

	content.setTextureRect(sf::IntRect(scrollHorizontal, scrollVertical, style[state].size.x, style[state].size.y));
	contentRedraw = false;
}
bool GUI_Interface::NeedsControlRedraw() const{ return controlRedraw; }
void GUI_Interface::RedrawControls(){
	if (controlTexture->getSize().x != style[state].size.x || controlTexture->getSize().y != style[state].size.y)
	{
		controlTexture->create(style[state].size.x, style[state].size.y);
	}
	controlTexture->clear(sf::Color(0, 0, 0, 0));

	for (auto &itr : elements){
		GUI_Element* element = itr.second;
		if (!element->IsActive() || !element->IsControl()){ continue; }
		element->ApplyStyle();
		element->Draw(controlTexture);
		element->SetRedraw(false);
	}

	controlTexture->display();
	control.setTexture(controlTexture->getTexture());
	control.setTextureRect(sf::IntRect(0, 0, style[state].size.x, style[state].size.y));
	controlRedraw = false;
}

void GUI_Interface::ToggleTitleBar(){ showTitleBar = !showTitleBar; }

void GUI_Interface::AdjustContentSize(const GUI_Element* reference){
	if (reference){
		sf::Vector2f bottomRight = reference->GetPosition() + reference->GetSize();
		if (bottomRight.x > contentSize.x){ contentSize.x = bottomRight.x; controlRedraw = true; }
		if (bottomRight.y > contentSize.y){ contentSize.y = bottomRight.y; controlRedraw = true; }
		return;
	}

	sf::Vector2f farthest = GetSize();

	for (auto &itr : elements){
		GUI_Element* element = itr.second;
		if (!element->IsActive() || element->IsControl()){ continue; }
		sf::Vector2f bottomRight = element->GetPosition() + element->GetSize();
		if (bottomRight.x > farthest.x){ farthest.x = bottomRight.x; controlRedraw = true; }
		if (bottomRight.y > farthest.y){ farthest.y = bottomRight.y; controlRedraw = true; }
	}
	SetContentSize(farthest);
}

void GUI_Interface::SetContentSize(const sf::Vector2f& vec){ contentSize = vec; }

void GUI_Interface::UpdateScrollHorizontal(unsigned int percent){
	if (percent > 100){ return; }
	scrollHorizontal = ((contentSize.x - GetSize().x) / 100) * percent;
	sf::IntRect rect = content.getTextureRect();
	content.setTextureRect(sf::IntRect(scrollHorizontal, scrollVertical, rect.width, rect.height));
}

void GUI_Interface::UpdateScrollVertical(unsigned int percent){
	if (percent > 100){ return; }
	scrollVertical = ((contentSize.y - GetSize().y) / 100) * percent;
	sf::IntRect rect = content.getTextureRect();
	content.setTextureRect(sf::IntRect(scrollHorizontal, scrollVertical, rect.width, rect.height));
}

const sf::Vector2f& GUI_Interface::GetContentSize() const{ return contentSize; }

void GUI_Interface::DefocusTextfields(){
	GUI_Event event;
	event.type = GUI_EventType::Release;
	event.interface = name.c_str();
	event.element = "";
	for (auto &itr : elements){
		if (itr.second->GetType() != GUI_ElementType::Textfield){ continue; }
		itr.second->SetState(GUI_ElementState::Neutral);
		event.element = itr.second->name.c_str();
		guiManager->AddEvent(event);
	}
}