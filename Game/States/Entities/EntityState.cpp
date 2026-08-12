#include "EntityState.hpp"

#include "Components/MovementGridComponent.hpp"


using namespace Game::States;


EntityState::EntityState(bae::GameObject& owner) :
    m_Owner{ &owner }
{
}

void EntityState::OnResume()
{
    m_bIsPaused = false;

    const auto movementComponent = m_Owner->GetComponent<MovementGridComponent>();
    if(movementComponent)
    {
        movementComponent->Enable();
    }
}

void EntityState::OnSuspend()
{
    m_bIsPaused = true;

    const auto movementComponent = m_Owner->GetComponent<MovementGridComponent>();
    if(movementComponent)
    {
        movementComponent->Disable();
    }
}

