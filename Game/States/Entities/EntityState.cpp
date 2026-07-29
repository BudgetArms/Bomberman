#include "EntityState.hpp"


using namespace Game::States;


EntityState::EntityState(bae::GameObject& owner) :
    m_Owner{ &owner }
{
}

void EntityState::OnResume()
{
    m_bIsPaused = false;

    // TODO: finish the logic
    /*
    auto movementComponent = m_Owner->GetComponent<MovementComponent>();
    if(movementComponent)
    {
        movementComponent->Enable();
    }
    */
}

void EntityState::OnSuspend()
{
    m_bIsPaused = true;

    // TODO: finish the logic
    /*
    auto movementComponent = m_Owner->GetComponent<MovementComponent>();
    if(movementComponent)
    {
        movementComponent->Disable();
    }
    */
}

