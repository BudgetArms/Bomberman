#pragma once

#include <memory>

#include "Core/GameObject.hpp"


namespace Game::States
{
    class EntityState
    {
    public:
        explicit EntityState(bae::GameObject& owner);

        virtual ~EntityState() = default;

        virtual void OnEnter() = 0;
        virtual void OnExit() = 0;

        virtual void OnResume();
        virtual void OnSuspend();

        virtual std::unique_ptr<EntityState> Update() = 0;

    protected:
        bae::GameObject* m_Owner;
        bool m_bIsPaused{ false };
    };
}
