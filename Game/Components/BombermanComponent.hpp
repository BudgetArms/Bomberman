#pragma once

#include "Components/Component.hpp"
#include "Core/Subject.hpp"

#include "States/Entities/EntityState.hpp"


namespace Game
{
    class BombermanComponent final : public bae::Component, public bae::Subject
    {
    public:
        explicit BombermanComponent(bae::GameObject& owner);

        void Update() override;


        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);


        void TryPlaceBomb() const;
        void TryActivateBomb() const;

    private:
        std::unique_ptr<States::EntityState> m_State{};
    };
}

