#pragma once

#include <filesystem>

#include <nlohmann/json.hpp>

#include "Components/Component.hpp"
#include "Core/SpriteSheet.hpp"

#include "Base/GameMode.hpp"
#include "Core/EventListener.hpp"


namespace Game
{
    class LevelManager final : public bae::Singleton<LevelManager>, public bae::EventListener
    {
    public:
        explicit LevelManager();
        ~LevelManager() override;

        void SpawnBomberman();
        void SpawnBombermiss();

        std::shared_ptr<bae::GameObject> GetBombermanBase(const std::string& gameObjectName,
                                                          const glm::vec2& spawnPosition);
        void HandleEvent(unsigned) override {};

        /*
        void RenderBackground() const;

        void LoadLevelFromFile(int levelNumber, const std::filesystem::path& jsonFile);

        void CreateLevel();
        void ResetLevel();

        void SkipLevel();


        [[nodiscard]] int GetCurrentLevelNumber() const;
        [[nodiscard]] LevelJson GetCurrentLevel();

        void SetGameMode(GameMode gameMode);

        void SetSpriteSheetWorldLocation(const glm::vec2& location) const;
        void SetSpriteSheetWorldRotation(float rotation) const;
        void SetSpriteSheetWorldScale(const glm::vec2& scale) const;

    private:
        void WonLevel();
        void LostLevel();

        void LoadBackground() const;

        void ClearLevel() const;

        [[nodiscard]] std::optional<LevelJson> GetCurrentLevelJson();

        void CreateGrid();

        void HandleEvent(unsigned int eventHash) override;
        void HandlePlayerDied() const;

        static void AddControls(bae::GameObject& gameObject, bool firstPlayer);
    */

        std::unique_ptr<bae::SpriteSheet> m_BackgroundSpriteSheet{};

        GameMode m_GameMode{ GameMode::Singleplayer };


        int m_CurrentLevel{};

        // std::unordered_map<int, LevelJson> m_LevelJson{};

        const std::filesystem::path m_BackgroundTexturePath{ "Textures/Level/Levels.png" };
    };
}
