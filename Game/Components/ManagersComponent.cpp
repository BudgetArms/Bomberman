#include "ManagersComponent.hpp"

#include "Managers/CollisionManager.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/ScenesManager.hpp"


Game::ManagersComponent::ManagersComponent(bae::GameObject& owner) :
    Component(owner)
{
}

Game::ManagersComponent::~ManagersComponent()
{
    ScenesManager::GetInstance().Destroy();
    CollisionManager::GetInstance().Destroy();
    LevelManager::GetInstance().Destroy();
}

void Game::ManagersComponent::Update()
{
}

void Game::ManagersComponent::FixedUpdate()
{
    CollisionManager::GetInstance().HandleCollisions();
}

void Game::ManagersComponent::LateUpdate()
{
    ScenesManager::GetInstance().Update();
}

void Game::ManagersComponent::Render() const
{
    LevelManager::GetInstance().RenderBackground();
}
