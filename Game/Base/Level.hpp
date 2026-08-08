#pragma once

#include <set>
#include <nlohmann/json.hpp>

#include "Core/Renderer.hpp"
#include "Graphs/GridGraph.hpp"


namespace Game
{
    enum class ScoreType
    {
        Pickup,
        BalloomKill,
        OnealKill,
        DollKill,
        MinvoKill,
    };

    struct LevelInfo
    {
        std::string Name{};
        int Index{};

        int GridNrColumns{};
        int GridNrRows{};
        glm::ivec2 GridCellSize{};
        glm::vec2 GridOffset{};

        glm::vec2 HitboxDimensions{};

        int BombermanStartLives{};
        float BombermanSpeed{};
        bae::Graphs::GridPosition BombermanPosition{};

        int BombermissStartLives{};
        float BombermissSpeed{};
        bae::Graphs::GridPosition BombermissPosition{};

        float BalloomPlayerSpeed{};
        bae::Graphs::GridPosition BalloomPlayerPosition{};

        float BalloomSpeed{};
        float OnealSpeed{};
        float DollSpeed{};
        float MinvoSpeed{};

        float BalloomDirectionUpChance{};
        float OnealDirectionUpChance{};
        float DollDirectionUpChance{};
        float MinvoDirectionUpChance{};

        bae::Graphs::GridPosition DoorPosition{};

        std::unordered_map<ScoreType, int> ScoreMap{};

        bae::Graphs::GridPosition PickupBombPosition{};
        bae::Graphs::GridPosition PickupFirePosition{};
        bae::Graphs::GridPosition PickupRemoteControlPosition{};

        std::set<bae::Graphs::GridPosition> BalloomPositions{};
        std::set<bae::Graphs::GridPosition> OnealPositions{};
        std::set<bae::Graphs::GridPosition> DollPositions{};
        std::set<bae::Graphs::GridPosition> MinvoPositions{};

        std::set<bae::Graphs::GridPosition> PermanentBlockPositions{};
        std::set<bae::Graphs::GridPosition> TemporaryBlockPositions{};
    };


    void from_json(const nlohmann::json& json, LevelInfo& level);
}
