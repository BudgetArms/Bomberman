#include "InputLeaderboardNameState.hpp"

#include "Base/Events.hpp"
#include "States/UI/MainMenuState.hpp"


using namespace Game::States;


InputLeaderboardNameState::InputLeaderboardNameState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void InputLeaderboardNameState::OnEnter()
{
    ClearCommands();
    AddSelectionConfirmedCommands();
    AddSelectionDownUpCommands();
    AddSelectionLeftRightCommands();
}

void InputLeaderboardNameState::OnExit()
{
    ClearCommands();
}

std::unique_ptr<SceneState> InputLeaderboardNameState::Update()
{
    if(!m_bIsSelected)
    {
        return nullptr;
    }

    [[maybe_unused]] const std::string leaderboardName = GetLeaderboardName();
    //

    return std::make_unique<MainMenuState>(*m_GameObject);
}

void InputLeaderboardNameState::Notify(const unsigned eventHash, bae::Subject*, const std::any&)
{
    const Events event = GetEvent(eventHash);

    if(event == Events::SelectionConfirmed)
    {
        m_bIsSelected = true;
        return;
    }

    if(event == Events::SelectionLeft)
    {
        if(m_LetterIndex == 0)
        {
            return;
        }

        --m_LetterIndex;
    }

    if(event == Events::SelectionRight)
    {
        if(m_LetterIndex >= m_NrLetters - 1)
        {
            return;
        }

        ++m_LetterIndex;
    }

    if(event == Events::SelectionUp)
    {
        if(m_LeaderboardName[m_LetterIndex]->m_Index == 0)
        {
            m_LeaderboardName[m_LetterIndex]->m_Index = 25;
            return;
        }

        --m_LeaderboardName[m_LetterIndex]->m_Index;
    }

    if(event == Events::SelectionDown)
    {
        if(m_LeaderboardName[m_LetterIndex]->m_Index == 25)
        {
            m_LeaderboardName[m_LetterIndex]->m_Index = 0;
            return;
        }

        ++m_LeaderboardName[m_LetterIndex]->m_Index;
    }
}

std::string InputLeaderboardNameState::GetLetter(const int letterIndex)
{
    return std::basic_string<char>{ 1, static_cast<char>('a' + letterIndex) };
}


std::string InputLeaderboardNameState::GetLeaderboardName() const
{
    std::string leaderboardName{};
    for(const auto& leaderboardSpriteSheet : m_LeaderboardName)
    {
        leaderboardName.append(GetLetter(leaderboardSpriteSheet->m_Index));
    }

    std::cout << FUNCTION_NAME << " " << leaderboardName << '\n';
    return leaderboardName;
}

