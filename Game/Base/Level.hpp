#pragma once

#include <set>
#include <nlohmann/json.hpp>

#include "Core/Renderer.hpp"
#include "Graphs/GridGraph.hpp"


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

    enum class ItemType
    {
        Bomb,
        Fire,
        RemoteControl
    };

    enum class ScoreType
    {
        Pickup,
        BalloomKill,
        OnealKill,
        DollKill,
        MinvoKill,
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


    struct LevelInfo
    {
        std::string Name{};
        int Index{};

        glm::vec2 HitboxDimension{};

        GridInfo GridInfo{};
        PlayerInfo BombermanInfo{};
        PlayerInfo BombermissInfo{};
        PlayerInfo BalloomPlayerInfo{};

        std::unordered_map<EnemyType, SharedEnemyInfo> EnemySharedInfos{};
        std::unordered_map<EnemyType, std::vector<bae::Graphs::GridPosition>> EnemyStartPositions{};

        bae::Graphs::GridPosition DoorPosition{};

        std::unordered_map<ScoreType, int> ScoreMap{};

        std::unordered_map<ItemType, std::vector<bae::Graphs::GridPosition>> ItemPositions{};

        std::set<bae::Graphs::GridPosition> PermanentBlockPositions{};
        std::set<bae::Graphs::GridPosition> TemporaryBlockPositions{};
    };


    void from_json(const nlohmann::json& json, LevelInfo& level);

    bae::Graphs::GridPosition GetGridPosFromJson(nlohmann::basic_json<> data);
    glm::vec2 GetPosFromJson(nlohmann::basic_json<> data);
}
