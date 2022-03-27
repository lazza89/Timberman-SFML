#include "GUI_Element.h"
#include "GUI_Interface.h"
#include "GUI_Manager.h"
#include "SharedContext.h"

GUI_Element::GUI_Element(const std::string& name, const GUI_ElementType& type, GUI_Interface* owner) : 
	name(name),
	type(type),
	state(GUI_ElementState::Neutral),
	owner(owner),
	needsRedraw(false),
	active(true),
	isControl(false),
	position(sf::Vector2f(0, 0))
{}

GUI_Element::~GUI_Element(){ 
	ReleaseResources();
}

void GUI_Element::ReleaseResources(){
	for (auto &itr : style){
		ReleaseTexture(itr.second.backgroundImage);
		ReleaseTexture(itr.second.glyph);
		ReleaseFont(itr.second.textFont);
	}
}

void GUI_Element::UpdateStyle(const GUI_ElementState& stat, const GUI_Style& styl)
{
	// Resource management.
	if (styl.backgroundImage != style[stat].backgroundImage) {
		ReleaseTexture(style[stat].backgroundImage);
		RequireTexture(styl.backgroundImage);
	}

	if (styl.glyph != style[stat].glyph) {
		ReleaseTexture(style[stat].glyph);
		RequireTexture(styl.glyph);
	}

	if (styl.textFont != style[stat].textFont) {
		ReleaseFont(style[stat].textFont);
		RequireFont(styl.textFont);
	}
	// Style application.
	style[stat] = styl;
	if (stat == state) { SetRedraw(true); ApplyStyle(); }
}

void GUI_Element::ApplyStyle(){
	ApplyTextStyle();
	ApplyBgStyle();
	ApplyGlyphStyle();
	if (owner != this && !IsControl()){ owner->AdjustContentSize(this); }
}

void GUI_Element::ApplyTextStyle(){
	FontManager* fonts = owner->GetManager()->GetContext()->fontManager;
	const GUI_Style& CurrentStyle = style[state];
	if (CurrentStyle.textFont != ""){
		visual.text.setFont(*fonts->GetResource(CurrentStyle.textFont));
		visual.text.setFillColor(CurrentStyle.textColor);
		visual.text.setCharacterSize(CurrentStyle.textSize);
		if (CurrentStyle.textCenterOrigin){
			sf::FloatRect rect = visual.text.getLocalBounds();
			visual.text.setOrigin(rect.left + rect.width / 2.0f,
				rect.top + rect.height / 2.0f);
		} else {
			visual.text.setOrigin(0.f, 0.f);
		}
	}
	visual.text.setPosition(position + CurrentStyle.textPadding);
}
void GUI_Element::ApplyBgStyle(){
	TextureManager* textures = owner->GetManager()->GetContext()->textureManager;
	const GUI_Style& CurrentStyle = style[state];
	if (CurrentStyle.backgroundImage != ""){
		visual.backgroundImage.setTexture(*textures->GetResource(CurrentStyle.backgroundImage));
		visual.backgroundImage.setColor(CurrentStyle.backgroundImageColor);
	}
	visual.backgroundImage.setPosition(position);
	visual.backgroundSolid.setSize(sf::Vector2f(CurrentStyle.size));
	visual.backgroundSolid.setFillColor(CurrentStyle.backgroundColor);
	visual.backgroundSolid.setPosition(position);
}

void GUI_Element::ApplyGlyphStyle(){
	const GUI_Style& CurrentStyle = style[state];
	TextureManager* textures = owner->GetManager()->GetContext()->textureManager;
	if (CurrentStyle.glyph != ""){
		visual.glyph.setTexture(*textures->GetResource(CurrentStyle.glyph));
	}
	visual.glyph.setPosition(position + CurrentStyle.glyphPadding);
}

GUI_ElementType GUI_Element::GetType(){ return type; }

void GUI_Element::SetState(const GUI_ElementState& stat){
	if (state == stat){ 
		return;
	}
	state = stat;
	SetRedraw(true);
}

const std::string& GUI_Element::GetName() const{
	return name; 
}

void GUI_Element::SetName(const std::string& nam){ 
	name = nam;
}

const sf::Vector2f& GUI_Element::GetPosition() const{ 
	return position;

}

void GUI_Element::SetPosition(const sf::Vector2f& pos){
	position = pos;
	if (owner == nullptr || owner == this){ return; }
	const auto& padding = owner->GetPadding();
	if (position.x < padding.x){ position.x = padding.x; }
	if (position.y < padding.y){ position.y = padding.y; }
}

const sf::Vector2f& GUI_Element::GetSize() const{ 
	return style.at(state).size; 
}

GUI_ElementState GUI_Element::GetState() const{ 
	return state;
}

void GUI_Element::SetRedraw(const bool& redraw){ 
	needsRedraw = redraw;
}

bool GUI_Element::NeedsRedraw() const{ 
	return needsRedraw;
}

void GUI_Element::SetOwner(GUI_Interface* ownr){
	owner = ownr;
}

GUI_Interface* GUI_Element::GetOwner() const{ 
	return owner;
}

bool GUI_Element::HasOwner() const{ 
	return owner != nullptr; 
}

bool GUI_Element::IsActive() const{
	return active;
}

void GUI_Element::SetActive(const bool& actv){
	if (actv != active){
		active = actv;
		SetRedraw(true); 
	}
}

bool GUI_Element::IsControl() const{ 
	return isControl; 
}

std::string GUI_Element::GetText() const{ 
	return visual.text.getString();
}

void GUI_Element::SetText(const std::string& txt){
	visual.text.setString(txt); SetRedraw(true);
}

bool GUI_Element::IsInside(const sf::Vector2f& point) const{
	sf::Vector2f position = GetGlobalPosition();
	return(point.x >= position.x && point.y >= position.y && point.x <= position.x + style.at(state).size.x && point.y <= position.y + style.at(state).size.y);
}
sf::Vector2f GUI_Element::GetGlobalPosition() const{
	sf::Vector2f position = GetPosition();
	if (owner == nullptr || owner == this){
		return position;
	}

	position += owner->GetGlobalPosition();
	if (IsControl()){
		return position;
	}

	position.x -= owner->scrollHorizontal;
	position.y -= owner->scrollVertical;
	return position;
}

void GUI_Element::RequireTexture(const std::string& name){
	if (name == ""){ 
		return; 
	}
	owner->GetManager()->GetContext()->textureManager->RequireResource(name);
}

void GUI_Element::RequireFont(const std::string& name){
	if (name == ""){ 
		return; 
	}
	owner->GetManager()->GetContext()->fontManager->RequireResource(name);
}

void GUI_Element::ReleaseTexture(const std::string& name){
	if (name == ""){ 
		return;
	}
	owner->GetManager()->GetContext()->textureManager->ReleaseResource(name);
}

void GUI_Element::ReleaseFont(const std::string& name){
	if (name == ""){
		return; 
	}
	owner->GetManager()->GetContext()->fontManager->ReleaseResource(name);
}