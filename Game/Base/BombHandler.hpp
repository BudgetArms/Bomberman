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

    protected:
        virtual void PlaceBomb();
        virtual void ActivateBomb();


        bae::GameObject* m_BombHandlerOwner;
        std::vector<bae::GameObject*> m_PlacedBombs{};
        int m_NrBombAllowedContinuously{ 1 };
    };
}
