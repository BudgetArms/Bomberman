#pragma once

#include "Components/Component.hpp"

#include "States/Entities/EntityState.hpp"


namespace Game
{
    namespace States
    {
        class EntityState;
    }

    class BombermanComponent final : public bae::Component
    {
    public:
        explicit BombermanComponent(bae::GameObject& owner);

    private:
        std::unique_ptr<States::EntityState> m_State{};
    };
}

