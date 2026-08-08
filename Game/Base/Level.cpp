#include "Level.hpp"


using namespace Game;


void Game::from_json(const nlohmann::json& json, LevelInfo& level)
{
    level =
    {
        .Name  = json.at("Name"),
        .Index = json.at("Index"),

        .GridNrColumns = json.at("Grid").at("Columns"),
        .GridNrRows    = json.at("Grid").at("Rows"),

        .GridCellSize =
        {
            json.at("Grid").at("CellSize").at("Width"),
            json.at("Grid").at("CellSize").at("Height")
        },

        .GridOffset =
        {
            json.at("Grid").at("Offset").at("X"),
            json.at("Grid").at("Offset").at("Y")
        },

        .HitboxDimensions =
        {
            json.at("Hitbox").at("Width"),
            json.at("Hitbox").at("Height")
        },

        .BombermanStartLives =
        {
            json.at("Bomberman").at("StartLives"),
        },
        .BombermanSpeed =
        {
            json.at("Bomberman").at("Speed"),
        },
        .BombermanPosition =
        {
            .Column = json.at("Bomberman").at("Position").at("Column"),
            .Row    = json.at("Bomberman").at("Position").at("Row")
        },

        .BombermissStartLives =
        {
            json.at("Bombermiss").at("StartLives"),
        },
        .BombermissSpeed =
        {
            json.at("Bombermiss").at("Speed"),
        },
        .BombermissPosition =
        {
            .Column = json.at("Bombermiss").at("Position").at("Column"),
            .Row    = json.at("Bombermiss").at("Position").at("Row")
        },

        .BalloomPlayerSpeed =
        {
            json.at("BalloomPlayer").at("Speed"),
        },
        .BalloomPlayerPosition =
        {
            .Column = json.at("BalloomPlayer").at("Position").at("Column"),
            .Row    = json.at("BalloomPlayer").at("Position").at("Row"),
        },

        .BalloomSpeed =
        {
            json.at("EnemySpeed").at("Balloom"),
        },
        .OnealSpeed =
        {
            json.at("EnemySpeed").at("Oneal"),
        },
        .DollSpeed =
        {
            json.at("EnemySpeed").at("Doll"),
        },
        .MinvoSpeed =
        {
            json.at("EnemySpeed").at("Balloom"),
        },

        .BalloomDirectionUpChance =
        {
            json.at("EnemyChangeDirectionUp").at("Balloom"),
        },
        .OnealDirectionUpChance =
        {
            json.at("EnemyChangeDirectionUp").at("Oneal"),
        },
        .DollDirectionUpChance =
        {
            json.at("EnemyChangeDirectionUp").at("Doll"),
        },
        .MinvoDirectionUpChance =
        {
            json.at("EnemyChangeDirectionUp").at("Minvo"),
        },

        .DoorPosition =
        {
            .Column = json.at("DoorPosition").at("Column"),
            .Row    = json.at("DoorPosition").at("Row"),
        },

        .ScoreMap =
        {
            {
                ScoreType::Pickup, json.at("ScoreMap").at("Pickup")
            },
            {
                ScoreType::BalloomKill, json.at("ScoreMap").at("Balloom")
            },
            {
                ScoreType::OnealKill, json.at("ScoreMap").at("Oneal")
            },
            {
                ScoreType::DollKill, json.at("ScoreMap").at("Doll")
            },
            {
                ScoreType::MinvoKill, json.at("ScoreMap").at("Minvo")
            }
        },

        .PickupBombPosition =
        {
            .Column = json.at("Pickups").at("Bomb").at("Column"),
            .Row    = json.at("Pickups").at("Bomb").at("Row"),
        },
        .PickupFirePosition =
        {
            .Column = json.at("Pickups").at("Fire").at("Column"),
            .Row    = json.at("Pickups").at("Fire").at("Row"),
        },
        .PickupRemoteControlPosition =
        {
            .Column = json.at("Pickups").at("RemoteControl").at("Column"),
            .Row    = json.at("Pickups").at("RemoteControl").at("Row"),
        },
    };

    for(const auto& balloomPosition : json.at("BalloomPositions"))
    {
        const bae::Graphs::GridPosition gridPosition
        {
            .Column = balloomPosition.at("Column"),
            .Row    = balloomPosition.at("Row")
        };

        level.BalloomPositions.insert(gridPosition);
    }

    for(const auto& onealPosition : json.at("OnealPositions"))
    {
        const bae::Graphs::GridPosition gridPosition
        {
            .Column = onealPosition.at("Column"),
            .Row    = onealPosition.at("Row")
        };

        level.OnealPositions.insert(gridPosition);
    }

    for(const auto& dollPosition : json.at("DollPositions"))
    {
        const bae::Graphs::GridPosition gridPosition
        {
            .Column = dollPosition.at("Column"),
            .Row    = dollPosition.at("Row")
        };

        level.DollPositions.insert(gridPosition);
    }

    for(const auto& minvoPosition : json.at("MinvoPositions"))
    {
        const bae::Graphs::GridPosition gridPosition
        {
            .Column = minvoPosition.at("Column"),
            .Row    = minvoPosition.at("Row")
        };

        level.MinvoPositions.insert(gridPosition);
    }


    for(const auto& permanentBlockPosition : json.at("PermanentBlockPositions"))
    {
        bae::Graphs::GridPosition gridPosition
        {
            .Column = permanentBlockPosition.at("Column"),
            .Row    = permanentBlockPosition.at("Row"),
        };

        level.PermanentBlockPositions.insert(gridPosition);
    }

    for(const auto& temporaryBlockPosition : json.at("TemporaryBlockPositions"))
    {
        bae::Graphs::GridPosition gridPosition
        {
            .Column = temporaryBlockPosition.at("Column"),
            .Row    = temporaryBlockPosition.at("Row"),
        };

        level.TemporaryBlockPositions.insert(gridPosition);
    }
}
