#include "BombermanComponent.hpp"

#include "States/Entities/BombermanStates.hpp"


Game::BombermanComponent::BombermanComponent(bae::GameObject& owner) :
    Component(owner)
{
    m_State = std::make_unique<States::BombermanAliveState>(owner);
}

