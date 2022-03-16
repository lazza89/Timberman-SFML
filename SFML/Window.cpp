#include "Window.h"

Window::Window(){ 
	Setup("Window", sf::Vector2u(640,480));
}

Window::Window(const std::string& title, const sf::Vector2u& size){ 
	Setup(title,size);
}

Window::~Window(){ window.close(); }

void Window::Setup(const std::string& title, const sf::Vector2u& size){
	windowTitle = title;
	windowSize = size;
	isFullscreen = false;
	isDone = false;
	isFocused = true;

	eventManager.AddCallback(StateType(0),"Fullscreen_toggle",&Window::ToggleFullscreen,this);
	eventManager.AddCallback(StateType(0),"Window_close",&Window::Close,this);

	Create();
}

void Window::Create(){
	sf::Uint32 style = sf::Style::Default;
	if(isFullscreen){ style = sf::Style::Fullscreen; }

	window.create(sf::VideoMode(windowSize.x,windowSize.y,32),windowTitle,style);
}

void Window::BeginDraw(){ window.clear(sf::Color::Black); }
void Window::EndDraw(){ window.display(); }

bool Window::IsDone(){ return isDone; }
bool Window::IsFullscreen(){ return isFullscreen; }
bool Window::IsFocused(){ return isFocused; }

sf::RenderWindow* Window::GetRenderWindow(){ return &window; }
EventManager* Window::GetEventManager(){ return &eventManager; }
sf::Vector2u Window::GetWindowSize(){ return windowSize; }
sf::FloatRect Window::GetViewSpace(){
	sf::Vector2f viewCenter(window.getView().getCenter());
	sf::Vector2f viewSize(window.getView().getSize());
	sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}

void Window::ToggleFullscreen(EventDetails* details){
	isFullscreen = !isFullscreen;
	window.close();
	Create();
}

void Window::Close(EventDetails* details){ isDone = true; }

void Window::Update(){
	sf::Event event;

	while(window.pollEvent(event)){
		if (event.type == sf::Event::LostFocus){ isFocused = false; eventManager.SetFocus(false); }
		else if (event.type == sf::Event::GainedFocus){ isFocused = true; eventManager.SetFocus(true); }
		eventManager.HandleEvent(event);
	}

	eventManager.Update();
}