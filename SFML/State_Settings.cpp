#include "State_Settings.h"
#include "StateManager.h"

State_Settings::State_Settings(StateManager* manager) : 
	BaseState(manager),
	stringRes("1280 x 720")
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	window = stateMgr->GetContext()->window;
	gui = stateMgr->GetContext()->window->GetGui();
}

State_Settings::~State_Settings(){}

void State_Settings::OnCreate()
{
	sf::Vector2u windowPos = window->GetRenderWindow()->getSize();
	
	AudioManager* audioMgr = stateMgr->GetContext()->audioManager;
	buttonSound.setBuffer(*stateMgr->GetContext()->audioManager->GetResource("ButtonSound"));
	sliderSoundTest.setBuffer(*stateMgr->GetContext()->audioManager->GetResource("ButtonSound"));

	exitButton = tgui::Button::create();
	exitButton->setSize(150, 50);
	exitButton->setPosition(50, windowPos.y * 0.9);
	exitButton->setTextSize(22);
	exitButton->setText("Back");

	resolutionBox = tgui::ComboBox::create();
	resolutionBox->addItem("1920 x 1080");
	resolutionBox->addItem("1280 x 720");
	resolutionBox->addItem("800 x 600");
	resolutionBox->setSelectedItem("1280 x 720");
	resolutionBox->setSize(250, 50);
	resolutionBox->setTextSize(20);
	resolutionBox->setOrigin(0.5, 0.5);
	resolutionBox->setPosition(windowPos.x * 0.5, windowPos.y * 0.35);

	resolutionLabel = tgui::Label::create();
	resolutionLabel->setText("Game Resolution");
	resolutionLabel->setTextSize(30);
	resolutionLabel->setOrigin(0.5, 0.5);
	resolutionLabel->setPosition(resolutionBox->getPosition().x, resolutionBox->getPosition().y - resolutionLabel->getSize().y - 70);

	saveButton = tgui::Button::create();
	saveButton->setSize(150, 50);
	saveButton->setText("Save");
	saveButton->setTextSize(22);
	saveButton->setPosition(windowPos.x - 200, windowPos.y * 0.9);

	volumeLabel = tgui::Label::create();
	volumeLabel->setPosition(resolutionBox->getPosition().x, resolutionBox->getPosition().y + 110);
	volumeLabel->setOrigin(0.5, 0.5);
	volumeLabel->setTextSize(30);
	volumeLabel->setText("Volume: 70%");

	volumeSlider = tgui::Slider::create();
	volumeSlider->setPosition(volumeLabel->getPosition().x, volumeLabel->getPosition().y + 70);
	volumeSlider->setOrigin(0.5, 0.5);
	volumeSlider->setMinimum(0);
	volumeSlider->setMaximum(100);
	volumeSlider->setValue(70);

	volumeSlider->onValueChange([&](int n) { volumeLabel->setText("Volume: " + std::to_string(n) + "%");});
	//volumeSlider->onFocus([&]() { sliderSoundTest.setVolume(volumeSlider->getValue()); sliderSoundTest.play(); });

	//signals
	exitButton->onPress(&State_Settings::BackToMainMenu, this);
	saveButton->onPress(&State_Settings::SaveSettings, this);

	exitButton->onPress([&]() { buttonSound.play(); });
	saveButton->onPress([&]() { buttonSound.play(); });

	gui->add(exitButton);
	gui->add(saveButton);
	gui->add(resolutionBox);
	gui->add(resolutionLabel);
	gui->add(volumeSlider);
	gui->add(volumeLabel);
}

void State_Settings::OnDestroy()
{
}

void State_Settings::Activate()
{
	exitButton->setVisible(true);
	saveButton->setVisible(true);
	resolutionBox->setVisible(true);
	resolutionLabel->setVisible(true);
	volumeSlider->setVisible(true);
	volumeLabel->setVisible(true);
}

void State_Settings::Deactivate()
{
	exitButton->setVisible(false);
	saveButton->setVisible(false);
	resolutionBox->setVisible(false);
	resolutionLabel->setVisible(false);
	volumeSlider->setVisible(false);
	volumeLabel->setVisible(false);
}

void State_Settings::Update(const sf::Time& time){}

void State_Settings::Draw()
{
	gui->draw();
}

void State_Settings::BackToMainMenu()
{
	stateMgr->SwitchTo(StateType::MainMenu);
}

void State_Settings::ChangedResolution()
{
	sf::Vector2u windowPos = window->GetRenderWindow()->getSize();
	exitButton->setPosition(50, windowPos.y * 0.9);
	resolutionBox->setPosition(windowPos.x * 0.5, windowPos.y * 0.35);
	resolutionLabel->setPosition(resolutionBox->getPosition().x, resolutionBox->getPosition().y - resolutionLabel->getSize().y - 70);
	saveButton->setPosition(windowPos.x -200, windowPos.y * 0.9);
	volumeLabel->setPosition(resolutionBox->getPosition().x, resolutionBox->getPosition().y + 110);
	volumeSlider->setPosition(volumeLabel->getPosition().x, volumeLabel->getPosition().y + 70);
}

void State_Settings::SaveSettings()
{
	buttonSound.setVolume(volumeSlider->getValue());
	stateMgr->GetContext()->generalVolume = volumeSlider->getValue();

	if (resolutionBox->getSelectedItem() != stringRes) {
		tgui::String tmp = resolutionBox->getSelectedItem();

		if (tmp == "1920 x 1080") {
			window->ChangeResolution(sf::Vector2u(1920, 1080));
			ChangedResolution();
			stringRes = "1920 x 1080";
		}
		else if (tmp == "1280 x 720") {
			window->ChangeResolution(sf::Vector2u(1280, 720));
			ChangedResolution();
			stringRes = "1280 x 720";
		}
		else if (tmp == "800 x 600") {
			window->ChangeResolution(sf::Vector2u(800, 600));
			ChangedResolution();
			stringRes = "800 x 600";
		}
	}
}
