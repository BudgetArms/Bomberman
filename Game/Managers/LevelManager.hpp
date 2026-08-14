#pragma once

#include <filesystem>

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "Components/Component.hpp"
#include "Core/SpriteSheet.hpp"

#include "Base/GameMode.hpp"
#include "Base/Level.hpp"
#include "Components/GridComponent.hpp"
#include "Core/EventListener.hpp"
#include "Core/Observer.hpp"


namespace Game
{
    class LevelManager final : public bae::Singleton<LevelManager>, public bae::EventListener, public bae::Observer
    {
    public:
        explicit LevelManager();
        ~LevelManager() override;

        void StartGame(GameMode gameMode);
        void StopGame();

        void Destroy();

        void RenderBackground() const;

        void SkipLevel();


        std::vector<std::pair<bae::GameObject*, PlayerType>> GetPlayers();
        std::unordered_map<bae::GameObject*, EnemyType> GetEnemies();

        [[nodiscard]] GameMode GetGameMode() const;
        [[nodiscard]] int GetTotalScore();
        [[nodiscard]] glm::vec2 GetHitboxDimension() const;

        [[nodiscard]] GridComponent* GetGridComponent() const;

        [[nodiscard]] glm::vec2 ToPosition(bae::Graphs::GridPosition gridPosition) const;

        void HandleEvent(unsigned int eventHash) override;
        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

        void LoadLevelInfo(const std::filesystem::path& jsonFile);

        [[nodiscard]] std::unordered_map<EnemyType, SharedEnemyInfo> GetEnemySharedInfo() const;
        [[nodiscard]] std::unordered_map<ScoreType, int> GetScoreMap() const;
        [[nodiscard]] int GetNrBombsAllowContinuously() const;
        [[nodiscard]] int GetBombFireRange() const;
        [[nodiscard]] bool HasBombRemoteControl() const;

        void IncreaseNrBombsAllowedContinuously();
        void IncreaseBombFireRange();
        void EnabledRemoteControl();


        static constexpr std::string_view m_SaveFileName{ "Scores.json" };
        static constexpr std::string_view m_SaveEntryName{ "Name" };
        static constexpr std::string_view m_SaveEntryScore{ "Score" };
        static constexpr std::string_view m_SinglePlayerName{ "Singleplayer" };
        static constexpr std::string_view m_CoOpName{ "Co-Op" };
        static constexpr std::string_view m_VersusName{ "Versus" };

        static constexpr float m_GlobalScale{ 2.f };

    private:
        void HandleBomberDeath(const bae::GameObject& object);
        static void HandleGameOver();

        // Clears Level Blocks & All Items/Entities
        void ClearLevel();
        void RestartLevel();


        void CreateGrid();
        void AddPermanentWalls() const;
        void AddTemporaryWalls();


        void SpawnBomberman();
        void SpawnBombermiss();
        void SpawnBalloomPlayer();

        void SpawnBalloom(const glm::vec2& position);
        void SpawnOneal(const glm::vec2& position);
        void SpawnDoll(const glm::vec2& position);
        void SpawnMinvo(const glm::vec2& position);

        void SpawnItem(const glm::vec2& position, ItemType itemType);

        void SpawnPlayers();
        void SpawnEnemies();

        void SpawnItems();


        std::shared_ptr<bae::GameObject> GetBombermanBase(const glm::vec2& spawnPosition, PlayerType playerType);
        std::shared_ptr<bae::GameObject> GetEnemyBase(const glm::vec2& spawnPosition, EnemyType enemyType);

        void SpawnTemporaryWall(const glm::vec2& position);
        void SpawnDoor(const glm::vec2& position);

        static void AddControls(bae::GameObject& gameObject, bool bIsFirstPlayer, bool bIsEnemy = false);
        void AddLevelCommands() const;

        void SavePlayerData();
        void LoadPlayerData() const;

        void LoadStartLevelData();
        void LoadNewLevelData();


        static constexpr int m_NrOfLevels{ 3 };

        const bool m_bShowHitboxes{ false };
        const std::string m_BackgroundTexturePath{ "Textures/Level/Playfield.png" };
        bae::Texture2D* m_BackgroundTexture{};

        GameMode m_GameMode{ GameMode::Singleplayer };
        bool m_bHasGameStarted{};

        int m_NrCurrentLevel{};
        std::unordered_map<int, LevelInfo> m_LoadedLevels{};
        LevelInfo m_CurrentLevelInfo{};

        int m_BombermanLives{};
        int m_BombermanScore{};

        int m_BombermissLives{};
        int m_BombermissScore{};

        int m_NrBombsAllowedContinuously{ 1 };
        int m_BombFireRange{ 1 };
        bool m_bHasBombRemoteControl{};
        bae::GameObject* m_Bomberman{};
        bae::GameObject* m_Bombermiss{};
        bae::GameObject* m_BalloomPlayer{};

        std::unordered_map<bae::GameObject*, EnemyType> m_Enemies{};

        GridComponent* m_GridComponent{};
    };
}

