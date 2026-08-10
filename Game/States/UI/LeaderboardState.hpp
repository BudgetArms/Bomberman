#pragma once

#include "States/UI/SceneState.hpp"


namespace Game::States
{
    class LeaderboardState final : public SceneState
    {
    public:
        explicit LeaderboardState(bae::GameObject& selectionObject);

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<SceneState> Update() override;

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        void LoadSaveFileData();


        struct ScoreData
        {
            std::string Name{};
            int Score{};
        };

        bool m_bIsSelected{};

        std::vector<ScoreData> m_SingleplayerData{};
        std::vector<ScoreData> m_CoOpData{};
        std::vector<ScoreData> m_VersusData{};
    };
}

