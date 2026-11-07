#include "LogMousePositionCommand.hpp"

#include <SDL3/SDL_mouse.h>

#include "Core/Logger.hpp"

#include "Managers/LevelManager.hpp"


using namespace Game;


void LogMousePositionCommand::Execute()
{
    float x{};
    float y{};
    SDL_GetMouseState(&x, &y);

    bae::Logger::LogNormal("MousePos X: " + std::to_string(static_cast<int>(x))
        + ", Y: " + std::to_string(static_cast<int>(y)));

    const auto gridComponent = LevelManager::GetInstance().GetGridComponent();
    if(!gridComponent)
    {
        return;
    }

    const auto gridPos = gridComponent->GetGridPosition({ x, y });
    if(!gridComponent->IsValidGridPosition(gridPos))
    {
        bae::Logger::LogWarning("MousePos invalid GridPos, Column" + std::to_string(gridPos.Column)
            + ", Row: " + std::to_string(gridPos.Row));

        return;
    }

    bae::Logger::LogNormal("MousePos GridPos, Column: " + std::to_string(gridPos.Column)
        + ", Row: " + std::to_string(gridPos.Row));
}

