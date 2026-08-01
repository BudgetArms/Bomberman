#include "LeaderboardState.hpp"

#include "Base/Events.hpp"
#include "States/UI/MainMenuState.hpp"


using namespace Game::States;


LeaderboardState::LeaderboardState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void LeaderboardState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
}

void LeaderboardState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> LeaderboardState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    return std::make_unique<MainMenuState>(*m_GameObject);
}

void LeaderboardState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);
    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
    }
}

