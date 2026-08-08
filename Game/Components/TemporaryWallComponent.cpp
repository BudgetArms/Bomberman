#include "TemporaryWallComponent.hpp"

#include "HitboxComponent.hpp"
#include "Managers/LevelManager.hpp"


using namespace Game;

TemporaryWallComponent::TemporaryWallComponent(bae::GameObject& owner) :
    Component(owner)
{
    m_Owner->AddComponent<bae::SpriteComponent>(*m_Owner, m_TexturePath, SDL_FRect(0, 0, 112, 16), 7, 7);
    m_SpriteComponent = m_Owner->GetComponent<bae::SpriteComponent>();

    const auto gridComponent                     = LevelManager::GetInstance().GetGridComponent();
    const bae::Graphs::GridPosition gridPosition = gridComponent->GetGridPosition(m_Owner->GetWorldLocation());

    gridComponent->SetNodeType(gridPosition, LevelNodeType::TempBlock);
}

