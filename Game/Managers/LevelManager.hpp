#pragma once

#include <filesystem>
#include <set>

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
    enum class PlayerType
    {
        Bomberman,
        Bombermiss
    };

    enum class EnemyType
    {
        Balloom,
        Oneal,
        Doll,
        Minvo,
        BalloomPlayer
    };

    enum class PickupType
    {
        Bomb,
        Fire,
        RemoteControl
    };

    struct GridInfo
    {
        int NrColumns{};
        int NrRows{};
        glm::ivec2 CellSize{};
        glm::vec2 Offset{};
    };

    struct SharedEnemyInfo
    {
        float Speed{};
        float DirectionUpChance{};
    };

    struct PlayerInfo
    {
        bae::Graphs::GridPosition StartPosition{};
        int Lives{};
        float Speed{};
        int Score{};
    };

    class LevelManager final : public bae::Singleton<LevelManager>, public bae::EventListener, public bae::Observer
    {
    public:
        explicit LevelManager();
        ~LevelManager() override;

        void StartGame(GameMode gameMode);

        void RenderBackground() const;

        void SkipLevel();


        std::vector<std::pair<bae::GameObject*, PlayerType>> GetPlayers();
        std::unordered_map<bae::GameObject*, EnemyType> GetEnemies();

        [[nodiscard]] GameMode GetGameMode() const;
        [[nodiscard]] int GetTotalScore();

        [[nodiscard]] GridComponent* GetGridComponent() const;


        void HandleEvent(unsigned int eventHash) override;
        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

        void LoadLevelInfo(const std::filesystem::path& jsonFile);

    private:
        void HandleBomberDeath(const bae::GameObject& object);
        void HandleGameOver() const;

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

        std::shared_ptr<bae::GameObject> GetBombermanBase(const std::string& gameObjectName,
                                                          const glm::vec2& spawnPosition);

        std::shared_ptr<bae::GameObject> GetEnemyBase(const std::string& gameObjectName,
                                                      const glm::vec2& spawnPosition);

        void SpawnTemporaryWall(const glm::vec2& position);
        void SpawnDoor(const glm::vec2& position);

        static void AddControls(bae::GameObject& gameObject, bool bIsFirstPlayer);

        void SavePlayerData();
        void LoadPlayerData() const;

        void LoadStartLevelData();

        [[nodiscard]] glm::vec2 ToPosition(bae::Graphs::GridPosition gridPosition) const;


        GameMode m_GameMode{ GameMode::Singleplayer };
        bool m_bHasGameStarted{};

        int m_CurrentLevel{};

        std::unordered_map<int, LevelInfo> m_LoadedLevels{};

        static constexpr float m_GlobalScale{ 2.f };

        bae::GameObject* m_Bomberman{};
        bae::GameObject* m_Bombermiss{};

        std::unordered_map<bae::GameObject*, EnemyType> m_Enemies{};

        const std::string m_BackgroundTexturePath{ "Textures/Level/Playfield.png" };
        bae::Texture2D* m_BackgroundTexture{};

        GridComponent* m_GridComponent{};

        glm::vec2 m_HitboxDimension{};

        PlayerInfo m_BombermanInfo{};
        PlayerInfo m_BombermissInfo{};
        PlayerInfo m_BalloomPlayerInfo{};

        std::vector<std::pair<EnemyType, bae::Graphs::GridPosition>> m_EnemyStartPositions{};

        std::unordered_map<EnemyType, SharedEnemyInfo> m_EnemySharedInfos{};

        bae::Graphs::GridPosition m_DoorPosition{};

        std::unordered_map<ScoreType, int> m_ScoreMap{};

        std::unordered_map<PickupType, bae::Graphs::GridPosition> m_PickupPosition{};

        GridInfo m_GridInfo{};
        std::set<bae::Graphs::GridPosition> m_PermanentBlockPositions{};
        std::set<bae::Graphs::GridPosition> m_TemporaryBlockPositions{};
    };
}
