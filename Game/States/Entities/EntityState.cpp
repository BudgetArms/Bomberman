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

    if(const auto movementComponent = m_Owner->GetComponent<MovementGridComponent>())
    {
        movementComponent->Enable();
    }
}

void EntityState::OnSuspend()
{
    m_bIsPaused = true;

    if(const auto movementComponent = m_Owner->GetComponent<MovementGridComponent>())
    {
        movementComponent->Disable();
    }
}
