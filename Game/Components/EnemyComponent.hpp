#pragma once

#include "Components/Component.hpp"
#include "Components/SpriteComponent.hpp"

#include "Base/Level.hpp"
#include "Components/MovementGridComponent.hpp"
#include "States/Entities/EntityState.hpp"


namespace Game
{
    class EnemyComponent final : public bae::Component, public bae::Subject
    {
    public:
        explicit EnemyComponent(bae::GameObject& owner, EnemyType enemyType);

        void FixedUpdate() override;


        void SetSpriteAndGridComponent();

        [[nodiscard]] EnemyType GetType() const;
        [[nodiscard]] int GetScore() const;

    private:
        void UpdateToNewState(std::unique_ptr<States::EntityState> newState);


        std::unique_ptr<States::EntityState> m_State{};

        bae::SpriteComponent* m_SpriteComponent{};
        MovementGridComponent* m_MovementGridComponent{};

        EnemyType m_EnemyType;

        bool m_bIsDying{ false };
    };
}

