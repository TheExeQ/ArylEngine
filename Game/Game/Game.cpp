#include "Game.h"

#include <Aryl/Scene/Entity.h>
#include <Aryl/Scene/Scene.h>
#include <Aryl/Scene/SceneManager.h>

Game::Game()
{

}

Game::~Game()
{

}

void Game::OnAttach()
{
	Aryl::SceneManager::SetActiveScene(CreateRef<Aryl::Scene>("Game Scene"));
}

void Game::OnDetach()
{

}

void Game::OnEvent(Aryl::Event& event)
{
	Aryl::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Aryl::AppUpdateEvent>(YL_BIND_EVENT_FN(Game::OnUpdate));
}

bool Game::OnUpdate(Aryl::AppUpdateEvent& e)
{


	return false;
}

