#include "GUI_Manager.h"
#include "SharedContext.h"
#include "Utilities.h"

GUI_Manager::GUI_Manager(EventManager* evMgr, SharedContext* shared) : 
	eventMgr(evMgr),
	context(shared),
	currentState(StateType(0))
{
	RegisterElement<GUI_Label>(GUI_ElementType::Label);
	RegisterElement<GUI_Scrollbar>(GUI_ElementType::Scrollbar);
	RegisterElement<GUI_Textfield>(GUI_ElementType::Textfield);
	elemTypes.emplace("Label", GUI_ElementType::Label);
	elemTypes.emplace("Button", GUI_ElementType::Button);
	elemTypes.emplace("Scrollbar", GUI_ElementType::Scrollbar);
	elemTypes.emplace("TextField", GUI_ElementType::Textfield);
	elemTypes.emplace("Interface", GUI_ElementType::Window);

	eventMgr->AddCallback(StateType(0), "Mouse_Left", &GUI_Manager::HandleClick, this);
	eventMgr->AddCallback(StateType(0), "Mouse_Left_Release", &GUI_Manager::HandleRelease, this);
	eventMgr->AddCallback(StateType(0), "Text_Entered", &GUI_Manager::HandleTextEntered, this);
}

GUI_Manager::~GUI_Manager(){
	eventMgr->RemoveCallback(StateType(0), "Mouse_Left");
	eventMgr->RemoveCallback(StateType(0), "Mouse_Left_Release");
	eventMgr->RemoveCallback(StateType(0), "Text_Entered");

	for (auto &itr : interfaces){
		for (auto &itr2 : itr.second){
			delete itr2.second;
		}
	}
}
GUI_Interface* GUI_Manager::GetInterface(const StateType& state,
	const std::string& name)
{
	auto s = interfaces.find(state);
	if (s == interfaces.end()){ return nullptr; }
	auto i = s->second.find(name);
	return (i != s->second.end() ? i->second : nullptr);
}

bool GUI_Manager::RemoveInterface(const StateType& state,
	const std::string& name)
{
	auto s = interfaces.find(state);
	if (s == interfaces.end()){ return false; }
	auto i = s->second.find(name);
	if (i == s->second.end()){ return false; }
	delete i->second;
	return s->second.erase(name);
}
void GUI_Manager::SetCurrentState(const StateType& state){
	if (currentState == state){ return; }
	HandleRelease(nullptr);
	currentState = state;
}
SharedContext* GUI_Manager::GetContext(){ return context; }

void GUI_Manager::DefocusAllInterfaces(){
	auto state = interfaces.find(currentState);
	if (state == interfaces.end()){ return; }
	for (auto &itr : state->second){
		itr.second->Defocus();
	}
}

void GUI_Manager::HandleClick(EventDetails* details){
	auto state = interfaces.find(currentState);
	if (state == interfaces.end()){ return; }
	sf::Vector2i mousePos = eventMgr->GetMousePos(context->window->GetRenderWindow());
	for (auto itr = state->second.rbegin(); itr != state->second.rend(); ++itr){
		if (!itr->second->IsInside(sf::Vector2f(mousePos))){ continue; }
		if (!itr->second->IsActive()){ return; }
		itr->second->OnClick(sf::Vector2f(mousePos));
		itr->second->Focus();
		if (itr->second->IsBeingMoved()){ itr->second->BeginMoving(); }
		return;
	}
}

void GUI_Manager::HandleRelease(EventDetails* details){
	auto state = interfaces.find(currentState);
	if (state == interfaces.end()){ return; }
	for (auto &itr : state->second){
		GUI_Interface* i = itr.second;
		if (!i->IsActive()){ continue; }
		if (i->GetState() == GUI_ElementState::Clicked)
		{
			i->OnRelease();
		}
		if (i->IsBeingMoved()){ i->StopMoving(); }
	}
}

void GUI_Manager::HandleTextEntered(EventDetails* details){
	auto state = interfaces.find(currentState);
	if (state == interfaces.end()){ return; }
	for (auto &itr : state->second){
		if (!itr.second->IsActive()){ continue; }
		if (!itr.second->IsFocused()){ continue; }
		itr.second->OnTextEntered(details->textEntered);
		return;
	}
}

void GUI_Manager::AddEvent(GUI_Event event){
	events[currentState].push_back(event);
}

bool GUI_Manager::PollEvent(GUI_Event& event){
	if (events[currentState].empty()){ return false; }
	event = events[currentState].back();
	events[currentState].pop_back();
	return true;
}
void GUI_Manager::Update(float dT){
	sf::Vector2i mousePos = eventMgr->GetMousePos(context->window->GetRenderWindow());

	auto state = interfaces.find(currentState);
	if (state == interfaces.end()){ return; }
	for (auto itr = state->second.rbegin(); itr != state->second.rend(); ++itr){
		GUI_Interface* i = itr->second;
		if (!i->IsActive()){ continue; }
		i->Update(dT);
		if (i->IsBeingMoved()){ continue; }
		if (i->IsInside(sf::Vector2f(mousePos)))
		{
			if (i->GetState() == GUI_ElementState::Neutral){
				i->OnHover(sf::Vector2f(mousePos));
			}
			return;
		} else if (i->GetState() == GUI_ElementState::Focused){
			i->OnLeave();
		}
	}
}
void GUI_Manager::Render(sf::RenderWindow* wind){
	auto state = interfaces.find(currentState);
	if (state == interfaces.end()){ return; }
	for (auto &itr : state->second){
		GUI_Interface* i = itr.second;
		if (!i->IsActive()){ continue; }
		if (i->NeedsRedraw()){ i->Redraw(); }
		if (i->NeedsContentRedraw()){ i->RedrawContent(); }
		if (i->NeedsControlRedraw()){ i->RedrawControls(); }
		i->Draw(wind);
	}
}
GUI_Element* GUI_Manager::CreateElement(const GUI_ElementType& id, GUI_Interface* owner){
	if (id == GUI_ElementType::Window){ return new GUI_Interface("", this); }
	auto f = factory.find(id);
	return (f != factory.end() ? f->second(owner) : nullptr);
}

bool GUI_Manager::AddInterface(const StateType& state,
	const std::string& name)
{
	auto s = interfaces.emplace(state, GUI_Interfaces()).first;
	GUI_Interface* temp = new GUI_Interface(name, this);
	if (s->second.emplace(name, temp).second){ return true; }
	delete temp;
	return false;
}

bool GUI_Manager::LoadInterface(const StateType& state,
	const std::string& interface, const std::string& name)
{
	std::ifstream file;
	file.open("media/GUI_Interfaces/" + interface);
	std::string InterfaceName;

	if (!file.is_open()){
		std::cout << "! Failed to load: " << interface << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(file, line)){
		if (line[0] == '|'){ continue; }
		std::stringstream keystream(line);
		std::string key;
		keystream >> key;
		if (key == "Interface"){
			std::string style;
			keystream >> InterfaceName >> style;
			if (!AddInterface(state, name)){
				std::cout << "Failed adding interface: " << name << std::endl;
				return false;
			}
			GUI_Interface* i = GetInterface(state, name);
			keystream >> *i;
			if (!LoadStyle(style, i)){
				std::cout << "Failed loading style file: " << style << " for interface " << name << std::endl;
			}
			i->SetContentSize(i->GetSize());
		} else if (key == "Element"){
			if (InterfaceName == ""){
				std::cout << "Error: 'Element' outside or before declaration of 'Interface'!" << std::endl;
				continue;
			}
			std::string type;
			std::string name;
			sf::Vector2f position;
			std::string style;
			keystream >> type >> name >> position.x >> position.y >> style;
			GUI_ElementType eType = StringToType(type);
			if (eType == GUI_ElementType::None){
				std::cout << "Unknown element('" << name << "') type: '" << type << "'" << std::endl;
				continue;
			}

			GUI_Interface* i = GetInterface(state, name);
			if (!i){ continue; }
			if (!i->AddElement(eType, name)){ continue; }
			GUI_Element* e = i->GetElement(name);
			keystream >> *e;
			e->SetPosition(position);
			if (!LoadStyle(style, e)){
				std::cout << "Failed loading style file: " << style << " for element " << name << std::endl;
				continue;
			}
		}
	}
	file.close();
	return true;
}
bool GUI_Manager::LoadStyle(const std::string& file, GUI_Element* element){
	std::ifstream fileStream;
	fileStream.open("media/GUI_Styles/" + file);

	std::string currentState;
	GUI_Style ParentStyle;
	GUI_Style TemporaryStyle;
	if (!fileStream.is_open()){
		std::cout << "! Failed to load: " << file << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(fileStream, line)){
		if (line[0] == '|'){ continue; }
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == ""){ continue; }
		if (type == "State"){
			if (currentState != ""){
				std::cout << "Error: 'State' keyword found inside another state!" << std::endl;
				continue;
			}
			keystream >> currentState;
		} else if (type == "/State"){
			if (currentState == ""){
				std::cout << "Error: '/State' keyword found prior to 'State'!" << std::endl;
				continue;
			}
			GUI_ElementState state = GUI_ElementState::Neutral;
			if (currentState == "Hover"){ state = GUI_ElementState::Focused; }
			else if (currentState == "Clicked"){ state = GUI_ElementState::Clicked; }

			if (state == GUI_ElementState::Neutral){
				ParentStyle = TemporaryStyle;
				element->UpdateStyle(GUI_ElementState::Neutral, TemporaryStyle);
				element->UpdateStyle(GUI_ElementState::Focused, TemporaryStyle);
				element->UpdateStyle(GUI_ElementState::Clicked, TemporaryStyle);
			} else {
				element->UpdateStyle(state, TemporaryStyle);
			}
			TemporaryStyle = ParentStyle;
			currentState = "";
		} else {
			// Handling style information.
			if (currentState == ""){
				std::cout << "Error: '" << type << "' keyword found outside of a state!" << std::endl;
				continue;
			}
			if (type == "Size"){
				keystream >> TemporaryStyle.size.x >> TemporaryStyle.size.y;
			} else if (type == "BgColor"){
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				TemporaryStyle.backgroundColor = sf::Color(r,g,b,a);
			} else if (type == "BgImage"){
				keystream >> TemporaryStyle.backgroundImage;
			} else if (type == "BgImageColor"){
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				TemporaryStyle.backgroundImageColor = sf::Color(r, g, b, a);
			} else if (type == "TextColor"){
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				TemporaryStyle.textColor = sf::Color(r, g, b, a);
			} else if (type == "TextSize"){
				keystream >> TemporaryStyle.textSize;
			} else if (type == "TextOriginCenter"){
				TemporaryStyle.textCenterOrigin = true;
			} else if (type == "Font"){
				keystream >> TemporaryStyle.textFont;
			} else if (type == "TextPadding"){
				keystream >> TemporaryStyle.textPadding.x >> TemporaryStyle.textPadding.y;
			} else if (type == "ElementColor"){
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				TemporaryStyle.elementColor = sf::Color(r, g, b, a);
			} else if (type == "Glyph"){
				keystream >> TemporaryStyle.glyph;
			} else if (type == "GlyphPadding"){
				keystream >> TemporaryStyle.glyphPadding.x >> TemporaryStyle.glyphPadding.y;
			} else {
				std::cout << "Error: style tag '" << type << "' is unknown!" << std::endl;
			}
		}
	}
	fileStream.close();
	return true;
}

GUI_ElementType GUI_Manager::StringToType(const std::string& string){
	auto t = elemTypes.find(string);
	return (t != elemTypes.end() ? t->second : GUI_ElementType::None);
}