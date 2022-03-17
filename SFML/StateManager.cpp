#include "StateManager.h"

StateManager::StateManager(SharedContext* shared)
	: shared(shared)
{
	RegisterState<State_Intro>(StateType::Intro);
	RegisterState<State_MainMenu>(StateType::MainMenu);
	RegisterState<State_Game>(StateType::Game);
	RegisterState<State_Paused>(StateType::Paused);
	RegisterState<State_GameOver>(StateType::GameOver);
}

StateManager::~StateManager(){
	for (auto &itr : states){
		itr.second->OnDestroy();
		delete itr.second;
	}
}

void StateManager::Update(const sf::Time& time){
	if (states.empty()){ return; }
	if (states.back().second->IsTranscendent() && states.size() > 1){
		auto itr = states.end();
		while (itr != states.begin()){
			if (itr != states.end()){
				if (!itr->second->IsTranscendent()){
					break;
				}
			}
			--itr;
		}
		for (; itr != states.end(); ++itr){
			itr->second->Update(time);
		}
	} else {
		states.back().second->Update(time);
	}
}

void StateManager::Draw(){
	if (states.empty()){ return; }
	if(states.back().second->IsTransparent() && states.size() > 1){
		auto itr = states.end();
		while(itr != states.begin()){
			if(itr != states.end()){
				if(!itr->second->IsTransparent()){
					break;
				}
			}
			--itr;
		}
		for(; itr != states.end(); ++itr){
			shared->window->GetRenderWindow()->setView(itr->second->GetView());
			itr->second->Draw();
		}
	} else {
		states.back().second->Draw();
	}
}

SharedContext* StateManager::GetContext(){ return shared; }

bool StateManager::HasState(const StateType& type){
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == type){
			auto removed = std::find(toRemove.begin(), toRemove.end(), type);
			if (removed == toRemove.end()){ return true; }
			return false;
		}
	}
	return false;
}

void StateManager::ProcessRequests(){
	while (toRemove.begin() != toRemove.end()){
		RemoveState(*toRemove.begin());
		toRemove.erase(toRemove.begin());
	}
}

void StateManager::SwitchTo(const StateType& type){
	shared->eventManager->SetCurrentState(type);
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == type){
			states.back().second->Deactivate();
			StateType tmp_type = itr->first;
			BaseState* tmp_state = itr->second;
			states.erase(itr);
			states.emplace_back(tmp_type, tmp_state);
			tmp_state->Activate();
			shared->window->GetRenderWindow()->setView(tmp_state->GetView());
			return;
		}
	}

	// State with type wasn't found.
	if (!states.empty()){ states.back().second->Deactivate(); }
	CreateState(type);
	states.back().second->Activate();
	shared->window->GetRenderWindow()->setView(states.back().second->GetView());
}

void StateManager::Remove(const StateType& type){
	toRemove.push_back(type);
}

// Private methods.

void StateManager::CreateState(const StateType& type){
	auto newState = stateFactory.find(type);
	if (newState == stateFactory.end()){ return; }
	BaseState* state = newState->second();
	state->view = shared->window->GetRenderWindow()->getDefaultView();
	states.emplace_back(type, state);
	state->OnCreate();
}

void StateManager::RemoveState(const StateType& type){
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == type){
			itr->second->OnDestroy();
			delete itr->second;
			states.erase(itr);
			return;
		}
	}
}