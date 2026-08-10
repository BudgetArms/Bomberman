#include "GameState.hpp"

#include "Core/Scene.hpp"
#include "Wrappers/Keyboard.hpp"

#include "Base/CommonManagerVariables.hpp"
#include "Base/Events.hpp"
#include "Commands/ForceDamageCommand.hpp"
#include "Commands/ToggleMuteSoundsCommand.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/SceneManager.hpp"
#include "States/UI/InputLeaderboardNameState.hpp"


using namespace Game::States;


GameState::GameState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void GameState::OnEnter()
{
    // Added Sound Toggle
    const bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto toggleMuteSoundsCommand = std::make_unique<ToggleMuteSoundsCommand>();
    keyboard.AddKeyboardCommands(std::move(toggleMuteSoundsCommand), SDLK_F2, bae::InputManager::ButtonState::Down);


    // todo: remove forceDamage
    const auto player      = LevelManager::GetInstance().GetPlayers()[0].first;
    auto removeLifeCommand = std::make_unique<ForceDamageCommand>(*player);
    keyboard.AddKeyboardCommands(std::move(removeLifeCommand), SDLK_5, bae::InputManager::ButtonState::Down);
}

void GameState::OnExit()
{
    ClearCommands();
    ClearScene();

    // Remove Level Foreground
    const auto* levelScene = bae::SceneManager::GetInstance().GetScene(g_LevelSceneName.data());
    levelScene->RemoveAll();

    // Remove Level Background
    const auto* scenesManagerScene = bae::SceneManager::GetInstance().GetScene(g_LevelBackgroundName.data());
    scenesManagerScene->RemoveAll();

    LevelManager::GetInstance().StopGame();
}

std::unique_ptr<SceneState> GameState::Update()
{
    if(!m_bIsGameFinished)
    {
        return nullptr;
    }

    auto& levelManager = LevelManager::GetInstance();

    const GameMode gameMode = levelManager.GetGameMode();
    const int finalScore    = levelManager.GetTotalScore();

    return std::make_unique<InputLeaderboardNameState>(*m_GameObject, gameMode, finalScore);
}

void GameState::HandleEvent(const unsigned eventHash)
{
    if(GetEvent(eventHash) == Events::GameWon ||
        GetEvent(eventHash) == Events::GameOver)
    {
        m_bIsGameFinished = true;
    }
}

void GameState::Notify(unsigned, bae::Subject*, const std::any&)
{
}

