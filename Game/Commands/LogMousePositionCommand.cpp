#include "LogMousePositionCommand.hpp"

#include <iostream>

#include <SDL3/SDL_mouse.h>

#include "Managers/LevelManager.hpp"


using namespace Game;


void LogMousePositionCommand::Execute()
{
    float x{};
    float y{};
    SDL_GetMouseState(&x, &y);

    std::cout << "MousePos X: " << static_cast<int>(x) << ", Y: " << static_cast<int>(y) << '\n';

    const auto gridComponent = LevelManager::GetInstance().GetGridComponent();
    if(!gridComponent)
    {
        return;
    }

    const auto gridPos = gridComponent->GetGridPosition({ x, y });
    if(!gridComponent->IsValidGridPosition(gridPos))
    {
        std::cout << "not valid" << '\n';
    }

    std::cout << "GridPos Column: " << gridPos.Column << ", Row: " << gridPos.Row << '\n';
}

