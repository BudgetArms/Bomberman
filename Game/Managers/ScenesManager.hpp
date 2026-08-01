#pragma once

#include "Core/GameObject.hpp"
#include "Core/Observer.hpp"
#include "Singletons/Singleton.hpp"


namespace Game
{
    enum class SceneMode
    {
        MainMenu,
        GameModeSelection,
        Leaderboard,
        Game,
        InputLeaderboardName,
    };

    class ScenesManager final : public bae::Singleton<ScenesManager>, public bae::Observer
    {
    public:
        void Initialize();

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

        bae::GameObject* GetSelectionObject() const;

    private:
        SceneMode m_SceneMode{};

        bae::GameObject* m_SelectionObject{};

        bool m_bIsInitialized{};
    };
}

