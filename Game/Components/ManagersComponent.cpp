#include "ManagersComponent.hpp"

#include "Managers/CollisionManager.hpp"
#include "Managers/LevelManager.hpp"


Game::ManagersComponent::ManagersComponent(bae::GameObject& owner) :
    Component(owner)
{
}

void Game::ManagersComponent::Update()
{
}

void Game::ManagersComponent::FixedUpdate()
{
}

void Game::ManagersComponent::LateUpdate()
{
    CollisionManager::GetInstance().HandleCollisions();
}

void Game::ManagersComponent::Render() const
{
    // LevelManager::GetInstance().RenderBackground();
}
