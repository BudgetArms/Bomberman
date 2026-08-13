#pragma once

#include <vector>

#include "Core/GameObject.hpp"


namespace Game
{
    class BombHandler
    {
    public:
        explicit BombHandler(bae::GameObject& owner);
        virtual ~BombHandler() = default;

        virtual void UpdateBombs();

        virtual void TryPlaceBomb();
        virtual void TryActivateBomb();

    private:
        bool m_bIsEnabled{ true };

    protected:
        virtual void PlaceBomb();
        virtual void ActivateBomb();

        virtual void EnableBombHandler();
        virtual void DisableBombHandler();


        bae::GameObject* m_BombHandlerOwner;
        std::vector<bae::GameObject*> m_PlacedBombs{};
    };
}
