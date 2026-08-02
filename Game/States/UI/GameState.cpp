#include "GameState.hpp"

#include "Wrappers/Keyboard.hpp"

#include "Base/Events.hpp"
#include "Commands/ToggleMuteSoundsCommand.hpp"
#include "Managers/LevelManager.hpp"
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
}

void GameState::OnExit()
{
    ClearCommands();
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

