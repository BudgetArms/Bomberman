#include "SkipLevelCommand.hpp"

#include "Managers/LevelManager.hpp"


using namespace Game;


SkipLevelCommand::SkipLevelCommand() :
    Command()
{
}

void SkipLevelCommand::Execute()
{
    LevelManager::GetInstance().SkipLevel();
}

