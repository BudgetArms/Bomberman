#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>

#include "Components/Component.hpp"
#include "Core/SpriteSheet.hpp"

#include "Base/GameMode.hpp"
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

        void SpawnBomberman();
        void SpawnBombermiss();
        void SpawnBalloomPlayer();

        void SpawnBalloom(const glm::vec2& position);
        void SpawnOneal(const glm::vec2& position);
        void SpawnDoll(const glm::vec2& position);
        void SpawnMinvo(const glm::vec2& position);

        static std::shared_ptr<bae::GameObject> GetBombermanBase(const std::string& gameObjectName,
                                                                 const glm::vec2& spawnPosition);

        static std::shared_ptr<bae::GameObject> GetEnemyBase(const std::string& gameObjectName,
                                                             const glm::vec2& spawnPosition);

        void RenderBackground() const;

        void SpawnBlocks();
        void SkipLevel();

        // Clears Level Blocks & All Items/Entities
        void ClearLevel();

        std::set<bae::GameObject*> GetPlayers();
        std::set<bae::GameObject*> GetEnemies();

        /*
        void LoadLevelFromFile(int levelNumber, const std::filesystem::path& jsonFile);

        [[nodiscard]] int GetCurrentLevelNumber() const;
        [[nodiscard]] LevelJson GetCurrentLevel();

        [[nodiscard]] std::optional<LevelJson> GetCurrentLevelJson();

        void CreateGrid();
    */
        void HandleEvent(unsigned int eventHash) override;
        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

    private:
        void HandleBomberDeath(const bae::GameObject& object);

        void RestartLevel();

        void HandleGameOver() const;


        static void AddControls(bae::GameObject& gameObject, bool bIsFirstPlayer);

        void SavePlayerData();
        void LoadPlayerData() const;

        GameMode m_GameMode{ GameMode::Singleplayer };
        bool m_bHasGameStarted{};

        int m_CurrentLevel{};

        // std::unordered_map<int, LevelJson> m_LevelJson{};

        bae::GameObject* m_Bomberman{};
        bae::GameObject* m_Bombermiss{};

        std::set<bae::GameObject*> m_Enemies{};

        const std::string m_BackgroundTexturePath{ "Textures/Level/Playfield.png" };
        bae::Texture2D* m_BackgroundTexture{};
        bae::SpriteSheet* m_LevelBlockTest{};

        // todo: load from file
        int m_BombermanLives{ 4 };
        int m_BombermissLives{ 4 };

        int m_BombermanScore{};
        int m_BombermissScore{};

        glm::vec2 m_BombermanStartPosition{ 200, 300 };
        glm::vec2 m_BombermissStartPosition{ 200, 400 };

        glm::vec2 m_BalloomPlayerStartPosition{ 500, 128 };
    };
}
