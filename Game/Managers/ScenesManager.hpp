#pragma once

#include "Core/EventListener.hpp"
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

    private:
        static void ClearScene();

        void LoadControls();

        void LoadMainMenuScene();
        void LoadGameModeSelectionScene();
        void LoadLeaderboardScene();
        void LoadGameScene();
        void LoadInputLeaderboardNameScene();


        SceneMode m_SceneMode{};

        bool m_bIsInitialized{};
    };
}

