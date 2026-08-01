#include "SceneState.hpp"

#include "Wrappers/Controller.hpp"
#include "Wrappers/Keyboard.hpp"

#include "Base/Events.hpp"
#include "Commands/SelectionCommands.hpp"
#include "Managers/LevelManager.hpp"
#include "Managers/ScenesManager.hpp"


using namespace Game::States;


SceneState::SceneState(bae::GameObject& selectionObject) :
    m_GameObject{ &selectionObject }
{
}


// Main Menu
MainMenuState::MainMenuState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void MainMenuState::OnEnter()
{
    bae::GameObject* gameObject = ScenesManager::GetInstance().GetSelectionObject();

    constexpr bae::InputManager::ButtonState buttonState = bae::InputManager::ButtonState::Pressed;

    bae::Keyboard& keyboard = bae::InputManager::GetInstance().GetKeyboard();

    auto selectionConfirmationCommand = std::make_unique<SelectionConfirmedCommand>(*gameObject);
    auto selectionDownCommand         = std::make_unique<SelectionDownCommand>(*gameObject);
    auto selectionUpCommand           = std::make_unique<SelectionUpCommand>(*gameObject);
    auto selectionLeftCommand         = std::make_unique<SelectionLeftCommand>(*gameObject);
    auto selectionRightCommand        = std::make_unique<SelectionRightCommand>(*gameObject);

    keyboard.AddKeyboardCommands(std::move(selectionConfirmationCommand), m_KeySelectionConfirmation, buttonState);
    keyboard.AddKeyboardCommands(std::move(selectionDownCommand), m_keySelectionDown, buttonState);
    keyboard.AddKeyboardCommands(std::move(selectionUpCommand), m_keySelectionUp, buttonState);
    keyboard.AddKeyboardCommands(std::move(selectionLeftCommand), m_keySelectionLeft, buttonState);
    keyboard.AddKeyboardCommands(std::move(selectionRightCommand), m_keySelectionRight, buttonState);


    [[maybe_unused]] bae::Controller* controller = bae::InputManager::GetInstance().GetController(0);

    selectionConfirmationCommand = std::make_unique<SelectionConfirmedCommand>(*gameObject);
    selectionDownCommand         = std::make_unique<SelectionDownCommand>(*gameObject);
    selectionUpCommand           = std::make_unique<SelectionUpCommand>(*gameObject);
    selectionLeftCommand         = std::make_unique<SelectionLeftCommand>(*gameObject);
    selectionRightCommand        = std::make_unique<SelectionRightCommand>(*gameObject);

    #if WIN32
    controller->AddControllerCommands(std::move(selectionConfirmationCommand), m_ControllerSelectionConfirmation,
                                      buttonState);
    controller->AddControllerCommands(std::move(selectionDownCommand), m_ControllerSelectionDown, buttonState);
    controller->AddControllerCommands(std::move(selectionUpCommand), m_ControllerSelectionUp, buttonState);
    controller->AddControllerCommands(std::move(selectionLeftCommand), m_ControllerSelectionLeft, buttonState);
    controller->AddControllerCommands(std::move(selectionRightCommand), m_ControllerSelectionRight, buttonState);
    #endif
}

void MainMenuState::OnExit()
{
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


// Game Mode Selection
GameModeSelectionState::GameModeSelectionState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void GameModeSelectionState::OnEnter()
{
}

void GameModeSelectionState::OnExit()
{
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


// GameState
GameState::GameState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void GameState::OnEnter()
{
}

void GameState::OnExit()
{
}

std::unique_ptr<SceneState> GameState::Update()
{
    if(!m_bIsGameFinished)
    {
        return nullptr;
    }

    return std::make_unique<InputLeaderboardNameState>(*m_GameObject);
}

void GameState::HandleEvent(const unsigned eventHash)
{
    if(GetEvent(eventHash) == Events::GameWon ||
        GetEvent(eventHash) == Events::GameOver)
    {
        m_bIsGameFinished = true;
    }
}


// Leaderboard State
LeaderboardState::LeaderboardState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void LeaderboardState::OnEnter()
{
}

void LeaderboardState::OnExit()
{
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


// Input Leaderboard Name
InputLeaderboardNameState::InputLeaderboardNameState(bae::GameObject& selectionObject) :
    SceneState(selectionObject)
{
}

void InputLeaderboardNameState::OnEnter()
{
}

void InputLeaderboardNameState::OnExit()
{
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
    return std::string(1, static_cast<char>('a' + letterIndex));
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
