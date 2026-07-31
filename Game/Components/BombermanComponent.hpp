#pragma once

#include "Components/Component.hpp"
#include "Core/Observer.hpp"
#include "Core/Subject.hpp"

#include "States/Entities/EntityState.hpp"


namespace Game
{
    class BombermanComponent final : public bae::Component, public bae::Observer, public bae::Subject
    {
    public:
        explicit BombermanComponent(bae::GameObject& owner);

        void Update() override;


        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);

        void Notify(unsigned eventHash, Subject* subject, const std::any& eventData) override;

        void HandleCollision(const std::any& eventData) const;
        void HandleEnemyCollision(bae::GameObject* gameObject) const;

    private:
        std::unique_ptr<States::EntityState> m_State{};
    };
}

