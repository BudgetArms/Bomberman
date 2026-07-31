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

        void SpawnBomberman();
        void SpawnBombermiss();

        static std::shared_ptr<bae::GameObject> GetBombermanBase(const std::string& gameObjectName,
                                                                 const glm::vec2& spawnPosition);

        void RenderBackground() const;

        void GenerateLevel(int level);
        void SkipLevel();

        void ClearLevel();
        /*
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

    */
        void HandleEvent(unsigned int eventHash) override;
        void HandleBomberLifeChanged(bae::GameObject& object);
        void HandleBomberDeath(bae::GameObject& object);

        void Notify(unsigned eventHash, bae::Subject* subject, const std::any& eventData) override;

        // static void AddControls(bae::GameObject& gameObject, bool firstPlayer);


        GameMode m_GameMode{ GameMode::Singleplayer };


        int m_CurrentLevel{};

        // std::unordered_map<int, LevelJson> m_LevelJson{};

        bae::GameObject* m_Bomberman{};
        bae::GameObject* m_Bombermiss{};

        const std::string m_BackgroundTexturePath{ "Textures/Level/Playfield.png" };
        bae::Texture2D* m_BackgroundTexture{};

        // todo: load from file
        int m_BombermanLives{ 4 };
        int m_BombermissLives{ 4 };

        glm::vec2 m_BombermanStartPosition{ 200, 300 };
        glm::vec2 m_BombermissStartPosition{ 200, 400 };
    };
}
