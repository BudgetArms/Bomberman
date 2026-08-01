#include "GameModeSelectionState.hpp"

#include "Base/Events.hpp"
#include "Managers/LevelManager.hpp"
#include "States/UI/GameState.hpp"
#include "States/UI/MainMenuState.hpp"


using namespace Game::States;


GameModeSelectionState::GameModeSelectionState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void GameModeSelectionState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();
}

void GameModeSelectionState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> GameModeSelectionState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    switch(m_Selection)
    {
        case GameModeSelection::SinglePlayer:
            LevelManager::GetInstance().StartGame(GameMode::Singleplayer);
            return std::make_unique<GameState>(*m_GameObject);
        case GameModeSelection::CoOp:
            LevelManager::GetInstance().StartGame(GameMode::CoOp);
            return std::make_unique<GameState>(*m_GameObject);
        case GameModeSelection::Versus:
            LevelManager::GetInstance().StartGame(GameMode::Versus);
            return std::make_unique<GameState>(*m_GameObject);
        case GameModeSelection::Back:
            return std::make_unique<MainMenuState>(*m_GameObject);
    }

    std::cout << FUNCTION_NAME << " This should never be reached" << '\n';
    return nullptr;
}

void GameModeSelectionState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);

    if(event != Events::SelectionUp &&
        event != Events::SelectionDown &&
        event != Events::SelectionConfirmed)
    {
        return;
    }

    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
    }

    if(event == Events::SelectionUp)
    {
        switch(m_Selection)
        {
            case GameModeSelection::SinglePlayer:
                return;
            case GameModeSelection::CoOp:
                m_Selection = GameModeSelection::SinglePlayer;
                return;
            case GameModeSelection::Versus:
                m_Selection = GameModeSelection::CoOp;
                return;
            case GameModeSelection::Back:
                m_Selection = GameModeSelection::Versus;
                return;
        }
    }

    if(event == Events::SelectionDown)
    {
        switch(m_Selection)
        {
            case GameModeSelection::SinglePlayer:
                m_Selection = GameModeSelection::CoOp;
                return;
            case GameModeSelection::CoOp:
                m_Selection = GameModeSelection::Versus;
                return;
            case GameModeSelection::Versus:
                m_Selection = GameModeSelection::Back;
                return;
            case GameModeSelection::Back:
                return;
        }
    }
}

