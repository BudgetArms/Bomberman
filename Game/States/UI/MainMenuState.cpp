#include "MainMenuState.hpp"

#include "Base/Events.hpp"
#include "States/UI/GameModeSelectionState.hpp"
#include "States/UI/LeaderboardState.hpp"


using namespace Game::States;


MainMenuState::MainMenuState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void MainMenuState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();
}

void MainMenuState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> MainMenuState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    switch(m_Selection)
    {
        case StartMenuSelections::Play:
            return std::make_unique<GameModeSelectionState>(*m_GameObject);
        case StartMenuSelections::Leaderboard:
            return std::make_unique<LeaderboardState>(*m_GameObject);
        case StartMenuSelections::Quit:
            return nullptr;
    }

    return nullptr;
}

void MainMenuState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
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
            case StartMenuSelections::Play:
                return;
            case StartMenuSelections::Leaderboard:
                m_Selection = StartMenuSelections::Play;
                return;
            case StartMenuSelections::Quit:
                m_Selection = StartMenuSelections::Leaderboard;
                return;
        }
    }

    if(event == Events::SelectionDown)
    {
        switch(m_Selection)
        {
            case StartMenuSelections::Play:
                m_Selection = StartMenuSelections::Leaderboard;
                return;
            case StartMenuSelections::Leaderboard:
                m_Selection = StartMenuSelections::Quit;
                return;
            case StartMenuSelections::Quit:
                return;
        }
    }
}

