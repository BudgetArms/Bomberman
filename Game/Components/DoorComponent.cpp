#include "DoorComponent.hpp"

#include "Components/HitboxComponent.hpp"
#include "Components/SpriteComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;


DoorComponent::DoorComponent(bae::GameObject& owner) :
    Component(owner)
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_TexturePath, SDL_FRect(0, 0, 16, 16), 1, 1);
    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();
}

