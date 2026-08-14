#include "Level.hpp"


using namespace Game;


void Game::from_json(const nlohmann::json& json, LevelInfo& level)
{
    level =
    {
        .Name  = json.at("Name"),
        .Index = json.at("Index"),

        .HitboxDimension = GetPosFromJson(json.at("Hitbox")),

        .GridInfo =
        {
            .NrColumns = json.at("Grid").at("Columns"),
            .NrRows    = json.at("Grid").at("Rows"),
            .CellSize  = {
                json.at("Grid").at("CellSize").at("Width"),
                json.at("Grid").at("CellSize").at("Height"),
            },
            .Offset = GetPosFromJson(json.at("Grid").at("Offset"))
        },

        .BombermanInfo =
        {
            .StartPosition = GetGridPosFromJson(json.at("Bomberman").at("Position")),
            .Lives         = json.at("Bomberman").at("StartLives"),
            .Speed         = json.at("Bomberman").at("Speed"),
            .Score         = json.at("Bomberman").at("Score")
        },

        .BombermissInfo =
        {
            .StartPosition = GetGridPosFromJson(json.at("Bombermiss").at("Position")),
            .Lives         = json.at("Bombermiss").at("StartLives"),
            .Speed         = json.at("Bombermiss").at("Speed"),
            .Score         = json.at("Bombermiss").at("Score")
        },

        .BalloomPlayerInfo =
        {
            .StartPosition = GetGridPosFromJson(json.at("BalloomPlayer").at("Position")),
            .Lives         = 0,
            .Speed         = json.at("BalloomPlayer").at("Speed"),
            .Score         = 0
        },

        .EnemySharedInfos =
        {
            {
                EnemyType::Balloom,
                SharedEnemyInfo
                {
                    .Speed             = json.at("SharedEnemyInfo").at("Balloom").at("Speed"),
                    .DirectionUpChance = json.at("SharedEnemyInfo").at("Balloom").at("ChangeDirectionUpChance"),
                    .Intelligence      = json.at("SharedEnemyInfo").at("Balloom").at("Intelligence"),
                }
            },
            {
                EnemyType::Oneal,
                SharedEnemyInfo
                {
                    .Speed             = json.at("SharedEnemyInfo").at("Oneal").at("Speed"),
                    .DirectionUpChance = json.at("SharedEnemyInfo").at("Oneal").at("ChangeDirectionUpChance"),
                    .Intelligence      = json.at("SharedEnemyInfo").at("Oneal").at("Intelligence"),
                }
            },
            {
                EnemyType::Doll,
                SharedEnemyInfo
                {
                    .Speed             = json.at("SharedEnemyInfo").at("Doll").at("Speed"),
                    .DirectionUpChance = json.at("SharedEnemyInfo").at("Doll").at("ChangeDirectionUpChance"),
                    .Intelligence      = json.at("SharedEnemyInfo").at("Doll").at("Intelligence"),
                }
            },
            {
                EnemyType::Minvo,
                SharedEnemyInfo
                {
                    .Speed             = json.at("SharedEnemyInfo").at("Minvo").at("Speed"),
                    .DirectionUpChance = json.at("SharedEnemyInfo").at("Minvo").at("ChangeDirectionUpChance"),
                    .Intelligence      = json.at("SharedEnemyInfo").at("Minvo").at("Intelligence"),
                }
            },
        },

        .EnemyStartPositions = {},

        .DoorPosition = GetGridPosFromJson(json.at("DoorPosition")),

        .ScoreMap =
        {
            {
                ScoreType::Pickup,
                json.at("ScoreMap").at("ItemPickup")
            },
            {
                ScoreType::BalloomKill,
                json.at("ScoreMap").at("BalloomKill")
            },
            {
                ScoreType::OnealKill,
                json.at("ScoreMap").at("OnealKill")
            },
            {
                ScoreType::DollKill,
                json.at("ScoreMap").at("DollKill")
            },
            {
                ScoreType::MinvoKill,
                json.at("ScoreMap").at("MinvoKill")
            },
        },

        .ItemPositions = {},

        .PermanentBlockPositions = {},
        .TemporaryBlockPositions = {}
    };


    // Enemy Start Positions
    for(const auto& balloomPosition : json.at("EnemyStartPositions").at("Balloom"))
    {
        level.EnemyStartPositions[EnemyType::Balloom].push_back(GetGridPosFromJson(balloomPosition));
    }
    for(const auto& onealPosition : json.at("EnemyStartPositions").at("Oneal"))
    {
        level.EnemyStartPositions[EnemyType::Oneal].push_back(GetGridPosFromJson(onealPosition));
    }
    for(const auto& dollPosition : json.at("EnemyStartPositions").at("Doll"))
    {
        level.EnemyStartPositions[EnemyType::Doll].push_back(GetGridPosFromJson(dollPosition));
    }
    for(const auto& minvoPosition : json.at("EnemyStartPositions").at("Minvo"))
    {
        level.EnemyStartPositions[EnemyType::Minvo].push_back(GetGridPosFromJson(minvoPosition));
    }

    // Item Positions
    for(const auto& bombPosition : json.at("ItemPositions").at("Bomb"))
    {
        level.ItemPositions[ItemType::Bomb].push_back(GetGridPosFromJson(bombPosition));
    }
    for(const auto& firePosition : json.at("ItemPositions").at("Fire"))
    {
        level.ItemPositions[ItemType::Fire].push_back(GetGridPosFromJson(firePosition));
    }
    for(const auto& remoteControlPosition : json.at("ItemPositions").at("RemoteControl"))
    {
        level.ItemPositions[ItemType::RemoteControl].push_back(GetGridPosFromJson(remoteControlPosition));
    }


    // Permanent Blocks
    for(const auto& permanentBlockPosition : json.at("PermanentBlockPositions"))
    {
        level.PermanentBlockPositions.insert(GetGridPosFromJson(permanentBlockPosition));
    }

    // Temporary Blocks
    for(const auto& temporaryBlockPosition : json.at("TemporaryBlockPositions"))
    {
        level.TemporaryBlockPositions.insert(GetGridPosFromJson(temporaryBlockPosition));
    }
}


bae::Graphs::GridPosition Game::GetGridPosFromJson(nlohmann::basic_json<> data)
{
    return bae::Graphs::GridPosition
    {
        data.at("Column"),
        data.at("Row"),
    };
}

glm::vec2 Game::GetPosFromJson(nlohmann::basic_json<> data)
{
    auto it = data.begin();

    return glm::vec2
    {
        (it++)->get<float>(),
        (it++)->get<float>()
    };
}
